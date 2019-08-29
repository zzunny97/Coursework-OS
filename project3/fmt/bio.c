4950 // Buffer cache.
4951 //
4952 // The buffer cache is a linked list of buf structures holding
4953 // cached copies of disk block contents.  Caching disk blocks
4954 // in memory reduces the number of disk reads and also provides
4955 // a synchronization point for disk blocks used by multiple processes.
4956 //
4957 // Interface:
4958 // * To get a buffer for a particular disk block, call bread.
4959 // * After changing buffer data, call bwrite to write it to disk.
4960 // * When done with the buffer, call brelse.
4961 // * Do not use the buffer after calling brelse.
4962 // * Only one process at a time can use a buffer,
4963 //     so do not keep them longer than necessary.
4964 //
4965 // The implementation uses two state flags internally:
4966 // * B_VALID: the buffer data has been read from the disk.
4967 // * B_DIRTY: the buffer data has been modified
4968 //     and needs to be written to disk.
4969 
4970 #include "types.h"
4971 #include "defs.h"
4972 #include "param.h"
4973 #include "spinlock.h"
4974 #include "sleeplock.h"
4975 #include "fs.h"
4976 #include "buf.h"
4977 
4978 struct {
4979   struct spinlock lock;
4980   struct buf buf[NBUF];
4981 
4982   // Linked list of all buffers, through prev/next.
4983   // head.next is most recently used.
4984   struct buf head;
4985 } bcache;
4986 
4987 void
4988 binit(void)
4989 {
4990   struct buf *b;
4991 
4992   initlock(&bcache.lock, "bcache");
4993 
4994 
4995 
4996 
4997 
4998 
4999 
5000   // Create linked list of buffers
5001   bcache.head.prev = &bcache.head;
5002   bcache.head.next = &bcache.head;
5003   for(b = bcache.buf; b < bcache.buf+NBUF; b++){
5004     b->next = bcache.head.next;
5005     b->prev = &bcache.head;
5006     initsleeplock(&b->lock, "buffer");
5007     bcache.head.next->prev = b;
5008     bcache.head.next = b;
5009   }
5010 }
5011 
5012 // Look through buffer cache for block on device dev.
5013 // If not found, allocate a buffer.
5014 // In either case, return locked buffer.
5015 static struct buf*
5016 bget(uint dev, uint blockno)
5017 {
5018   struct buf *b;
5019 
5020   acquire(&bcache.lock);
5021 
5022   // Is the block already cached?
5023   for(b = bcache.head.next; b != &bcache.head; b = b->next){
5024     if(b->dev == dev && b->blockno == blockno){
5025       b->refcnt++;
5026       release(&bcache.lock);
5027       acquiresleep(&b->lock);
5028       return b;
5029     }
5030   }
5031 
5032   // Not cached; recycle an unused buffer.
5033   // Even if refcnt==0, B_DIRTY indicates a buffer is in use
5034   // because log.c has modified it but not yet committed it.
5035   for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
5036     if(b->refcnt == 0 && (b->flags & B_DIRTY) == 0) {
5037       b->dev = dev;
5038       b->blockno = blockno;
5039       b->flags = 0;
5040       b->refcnt = 1;
5041       release(&bcache.lock);
5042       acquiresleep(&b->lock);
5043       return b;
5044     }
5045   }
5046   panic("bget: no buffers");
5047 }
5048 
5049 
5050 // Return a locked buf with the contents of the indicated block.
5051 struct buf*
5052 bread(uint dev, uint blockno)
5053 {
5054   struct buf *b;
5055 
5056   b = bget(dev, blockno);
5057   if((b->flags & B_VALID) == 0) {
5058     iderw(b);
5059   }
5060   return b;
5061 }
5062 
5063 // Write b's contents to disk.  Must be locked.
5064 void
5065 bwrite(struct buf *b)
5066 {
5067   if(!holdingsleep(&b->lock))
5068     panic("bwrite");
5069   b->flags |= B_DIRTY;
5070   iderw(b);
5071 }
5072 
5073 // Release a locked buffer.
5074 // Move to the head of the MRU list.
5075 void
5076 brelse(struct buf *b)
5077 {
5078   if(!holdingsleep(&b->lock))
5079     panic("brelse");
5080 
5081   releasesleep(&b->lock);
5082 
5083   acquire(&bcache.lock);
5084   b->refcnt--;
5085   if (b->refcnt == 0) {
5086     // no one is waiting for it.
5087     b->next->prev = b->prev;
5088     b->prev->next = b->next;
5089     b->next = bcache.head.next;
5090     b->prev = &bcache.head;
5091     bcache.head.next->prev = b;
5092     bcache.head.next = b;
5093   }
5094 
5095   release(&bcache.lock);
5096 }
5097 
5098 
5099 
5100 // Blank page.
5101 
5102 
5103 
5104 
5105 
5106 
5107 
5108 
5109 
5110 
5111 
5112 
5113 
5114 
5115 
5116 
5117 
5118 
5119 
5120 
5121 
5122 
5123 
5124 
5125 
5126 
5127 
5128 
5129 
5130 
5131 
5132 
5133 
5134 
5135 
5136 
5137 
5138 
5139 
5140 
5141 
5142 
5143 
5144 
5145 
5146 
5147 
5148 
5149 
