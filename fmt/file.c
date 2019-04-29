6400 //
6401 // File descriptors
6402 //
6403 
6404 #include "types.h"
6405 #include "defs.h"
6406 #include "param.h"
6407 #include "fs.h"
6408 #include "spinlock.h"
6409 #include "sleeplock.h"
6410 #include "file.h"
6411 
6412 struct devsw devsw[NDEV];
6413 struct {
6414   struct spinlock lock;
6415   struct file file[NFILE];
6416 } ftable;
6417 
6418 void
6419 fileinit(void)
6420 {
6421   initlock(&ftable.lock, "ftable");
6422 }
6423 
6424 // Allocate a file structure.
6425 struct file*
6426 filealloc(void)
6427 {
6428   struct file *f;
6429 
6430   acquire(&ftable.lock);
6431   for(f = ftable.file; f < ftable.file + NFILE; f++){
6432     if(f->ref == 0){
6433       f->ref = 1;
6434       release(&ftable.lock);
6435       return f;
6436     }
6437   }
6438   release(&ftable.lock);
6439   return 0;
6440 }
6441 
6442 
6443 
6444 
6445 
6446 
6447 
6448 
6449 
6450 // Increment ref count for file f.
6451 struct file*
6452 filedup(struct file *f)
6453 {
6454   acquire(&ftable.lock);
6455   if(f->ref < 1)
6456     panic("filedup");
6457   f->ref++;
6458   release(&ftable.lock);
6459   return f;
6460 }
6461 
6462 // Close file f.  (Decrement ref count, close when reaches 0.)
6463 void
6464 fileclose(struct file *f)
6465 {
6466   struct file ff;
6467 
6468   acquire(&ftable.lock);
6469   if(f->ref < 1)
6470     panic("fileclose");
6471   if(--f->ref > 0){
6472     release(&ftable.lock);
6473     return;
6474   }
6475   ff = *f;
6476   f->ref = 0;
6477   f->type = FD_NONE;
6478   release(&ftable.lock);
6479 
6480   if(ff.type == FD_PIPE)
6481     pipeclose(ff.pipe, ff.writable);
6482   else if(ff.type == FD_INODE){
6483     begin_op();
6484     iput(ff.ip);
6485     end_op();
6486   }
6487 }
6488 
6489 
6490 
6491 
6492 
6493 
6494 
6495 
6496 
6497 
6498 
6499 
6500 // Get metadata about file f.
6501 int
6502 filestat(struct file *f, struct stat *st)
6503 {
6504   if(f->type == FD_INODE){
6505     ilock(f->ip);
6506     stati(f->ip, st);
6507     iunlock(f->ip);
6508     return 0;
6509   }
6510   return -1;
6511 }
6512 
6513 // Read from file f.
6514 int
6515 fileread(struct file *f, char *addr, int n)
6516 {
6517   int r;
6518 
6519   if(f->readable == 0)
6520     return -1;
6521   if(f->type == FD_PIPE)
6522     return piperead(f->pipe, addr, n);
6523   if(f->type == FD_INODE){
6524     ilock(f->ip);
6525     if((r = readi(f->ip, addr, f->off, n)) > 0)
6526       f->off += r;
6527     iunlock(f->ip);
6528     return r;
6529   }
6530   panic("fileread");
6531 }
6532 
6533 
6534 
6535 
6536 
6537 
6538 
6539 
6540 
6541 
6542 
6543 
6544 
6545 
6546 
6547 
6548 
6549 
6550 // Write to file f.
6551 int
6552 filewrite(struct file *f, char *addr, int n)
6553 {
6554   int r;
6555 
6556   if(f->writable == 0)
6557     return -1;
6558   if(f->type == FD_PIPE)
6559     return pipewrite(f->pipe, addr, n);
6560   if(f->type == FD_INODE){
6561     // write a few blocks at a time to avoid exceeding
6562     // the maximum log transaction size, including
6563     // i-node, indirect block, allocation blocks,
6564     // and 2 blocks of slop for non-aligned writes.
6565     // this really belongs lower down, since writei()
6566     // might be writing a device like the console.
6567     int max = ((MAXOPBLOCKS-1-1-2) / 2) * 512;
6568     int i = 0;
6569     while(i < n){
6570       int n1 = n - i;
6571       if(n1 > max)
6572         n1 = max;
6573 
6574       begin_op();
6575       ilock(f->ip);
6576       if ((r = writei(f->ip, addr + i, f->off, n1)) > 0)
6577         f->off += r;
6578       iunlock(f->ip);
6579       end_op();
6580 
6581       if(r < 0)
6582         break;
6583       if(r != n1)
6584         panic("short filewrite");
6585       i += r;
6586     }
6587     return i == n ? n : -1;
6588   }
6589   panic("filewrite");
6590 }
6591 
6592 
6593 
6594 
6595 
6596 
6597 
6598 
6599 
