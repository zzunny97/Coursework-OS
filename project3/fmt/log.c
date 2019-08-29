5250 #include "types.h"
5251 #include "defs.h"
5252 #include "param.h"
5253 #include "spinlock.h"
5254 #include "sleeplock.h"
5255 #include "fs.h"
5256 #include "buf.h"
5257 
5258 // Simple logging that allows concurrent FS system calls.
5259 //
5260 // A log transaction contains the updates of multiple FS system
5261 // calls. The logging system only commits when there are
5262 // no FS system calls active. Thus there is never
5263 // any reasoning required about whether a commit might
5264 // write an uncommitted system call's updates to disk.
5265 //
5266 // A system call should call begin_op()/end_op() to mark
5267 // its start and end. Usually begin_op() just increments
5268 // the count of in-progress FS system calls and returns.
5269 // But if it thinks the log is close to running out, it
5270 // sleeps until the last outstanding end_op() commits.
5271 //
5272 // The log is a physical re-do log containing disk blocks.
5273 // The on-disk log format:
5274 //   header block, containing block #s for block A, B, C, ...
5275 //   block A
5276 //   block B
5277 //   block C
5278 //   ...
5279 // Log appends are synchronous.
5280 
5281 // Contents of the header block, used for both the on-disk header block
5282 // and to keep track in memory of logged block# before commit.
5283 struct logheader {
5284   int n;
5285   int block[LOGSIZE];
5286 };
5287 
5288 struct log {
5289   struct spinlock lock;
5290   int start;
5291   int size;
5292   int outstanding; // how many FS sys calls are executing.
5293   int committing;  // in commit(), please wait.
5294   int dev;
5295   struct logheader lh;
5296 };
5297 
5298 
5299 
5300 struct log log;
5301 
5302 static void recover_from_log(void);
5303 static void commit();
5304 
5305 void
5306 initlog(int dev)
5307 {
5308   if (sizeof(struct logheader) >= BSIZE)
5309     panic("initlog: too big logheader");
5310 
5311   struct superblock sb;
5312   initlock(&log.lock, "log");
5313   readsb(dev, &sb);
5314   log.start = sb.logstart;
5315   log.size = sb.nlog;
5316   log.dev = dev;
5317   recover_from_log();
5318 }
5319 
5320 // Copy committed blocks from log to their home location
5321 static void
5322 install_trans(void)
5323 {
5324   int tail;
5325 
5326   for (tail = 0; tail < log.lh.n; tail++) {
5327     struct buf *lbuf = bread(log.dev, log.start+tail+1); // read log block
5328     struct buf *dbuf = bread(log.dev, log.lh.block[tail]); // read dst
5329     memmove(dbuf->data, lbuf->data, BSIZE);  // copy block to dst
5330     bwrite(dbuf);  // write dst to disk
5331     brelse(lbuf);
5332     brelse(dbuf);
5333   }
5334 }
5335 
5336 // Read the log header from disk into the in-memory log header
5337 static void
5338 read_head(void)
5339 {
5340   struct buf *buf = bread(log.dev, log.start);
5341   struct logheader *lh = (struct logheader *) (buf->data);
5342   int i;
5343   log.lh.n = lh->n;
5344   for (i = 0; i < log.lh.n; i++) {
5345     log.lh.block[i] = lh->block[i];
5346   }
5347   brelse(buf);
5348 }
5349 
5350 // Write in-memory log header to disk.
5351 // This is the true point at which the
5352 // current transaction commits.
5353 static void
5354 write_head(void)
5355 {
5356   struct buf *buf = bread(log.dev, log.start);
5357   struct logheader *hb = (struct logheader *) (buf->data);
5358   int i;
5359   hb->n = log.lh.n;
5360   for (i = 0; i < log.lh.n; i++) {
5361     hb->block[i] = log.lh.block[i];
5362   }
5363   bwrite(buf);
5364   brelse(buf);
5365 }
5366 
5367 static void
5368 recover_from_log(void)
5369 {
5370   read_head();
5371   install_trans(); // if committed, copy from log to disk
5372   log.lh.n = 0;
5373   write_head(); // clear the log
5374 }
5375 
5376 // called at the start of each FS system call.
5377 void
5378 begin_op(void)
5379 {
5380   acquire(&log.lock);
5381   while(1){
5382     if(log.committing){
5383       sleep(&log, &log.lock);
5384     } else if(log.lh.n + (log.outstanding+1)*MAXOPBLOCKS > LOGSIZE){
5385       // this op might exhaust log space; wait for commit.
5386       sleep(&log, &log.lock);
5387     } else {
5388       log.outstanding += 1;
5389       release(&log.lock);
5390       break;
5391     }
5392   }
5393 }
5394 
5395 
5396 
5397 
5398 
5399 
5400 // called at the end of each FS system call.
5401 // commits if this was the last outstanding operation.
5402 void
5403 end_op(void)
5404 {
5405   int do_commit = 0;
5406 
5407   acquire(&log.lock);
5408   log.outstanding -= 1;
5409   if(log.committing)
5410     panic("log.committing");
5411   if(log.outstanding == 0){
5412     do_commit = 1;
5413     log.committing = 1;
5414   } else {
5415     // begin_op() may be waiting for log space,
5416     // and decrementing log.outstanding has decreased
5417     // the amount of reserved space.
5418     wakeup(&log);
5419   }
5420   release(&log.lock);
5421 
5422   if(do_commit){
5423     // call commit w/o holding locks, since not allowed
5424     // to sleep with locks.
5425     commit();
5426     acquire(&log.lock);
5427     log.committing = 0;
5428     wakeup(&log);
5429     release(&log.lock);
5430   }
5431 }
5432 
5433 // Copy modified blocks from cache to log.
5434 static void
5435 write_log(void)
5436 {
5437   int tail;
5438 
5439   for (tail = 0; tail < log.lh.n; tail++) {
5440     struct buf *to = bread(log.dev, log.start+tail+1); // log block
5441     struct buf *from = bread(log.dev, log.lh.block[tail]); // cache block
5442     memmove(to->data, from->data, BSIZE);
5443     bwrite(to);  // write the log
5444     brelse(from);
5445     brelse(to);
5446   }
5447 }
5448 
5449 
5450 static void
5451 commit()
5452 {
5453   if (log.lh.n > 0) {
5454     write_log();     // Write modified blocks from cache to log
5455     write_head();    // Write header to disk -- the real commit
5456     install_trans(); // Now install writes to home locations
5457     log.lh.n = 0;
5458     write_head();    // Erase the transaction from the log
5459   }
5460 }
5461 
5462 // Caller has modified b->data and is done with the buffer.
5463 // Record the block number and pin in the cache with B_DIRTY.
5464 // commit()/write_log() will do the disk write.
5465 //
5466 // log_write() replaces bwrite(); a typical use is:
5467 //   bp = bread(...)
5468 //   modify bp->data[]
5469 //   log_write(bp)
5470 //   brelse(bp)
5471 void
5472 log_write(struct buf *b)
5473 {
5474   int i;
5475 
5476   if (log.lh.n >= LOGSIZE || log.lh.n >= log.size - 1)
5477     panic("too big a transaction");
5478   if (log.outstanding < 1)
5479     panic("log_write outside of trans");
5480 
5481   acquire(&log.lock);
5482   for (i = 0; i < log.lh.n; i++) {
5483     if (log.lh.block[i] == b->blockno)   // log absorbtion
5484       break;
5485   }
5486   log.lh.block[i] = b->blockno;
5487   if (i == log.lh.n)
5488     log.lh.n++;
5489   b->flags |= B_DIRTY; // prevent eviction
5490   release(&log.lock);
5491 }
5492 
5493 
5494 
5495 
5496 
5497 
5498 
5499 
