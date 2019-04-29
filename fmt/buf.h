4400 struct buf {
4401   int flags;
4402   uint dev;
4403   uint blockno;
4404   struct sleeplock lock;
4405   uint refcnt;
4406   struct buf *prev; // LRU cache list
4407   struct buf *next;
4408   struct buf *qnext; // disk queue
4409   uchar data[BSIZE];
4410 };
4411 #define B_VALID 0x2  // buffer has been read from disk
4412 #define B_DIRTY 0x4  // buffer needs to be written to disk
4413 
4414 
4415 
4416 
4417 
4418 
4419 
4420 
4421 
4422 
4423 
4424 
4425 
4426 
4427 
4428 
4429 
4430 
4431 
4432 
4433 
4434 
4435 
4436 
4437 
4438 
4439 
4440 
4441 
4442 
4443 
4444 
4445 
4446 
4447 
4448 
4449 
