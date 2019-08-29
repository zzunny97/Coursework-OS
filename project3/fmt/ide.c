4750 // Simple PIO-based (non-DMA) IDE driver code.
4751 
4752 #include "types.h"
4753 #include "defs.h"
4754 #include "param.h"
4755 #include "memlayout.h"
4756 #include "mmu.h"
4757 #include "proc.h"
4758 #include "x86.h"
4759 #include "traps.h"
4760 #include "spinlock.h"
4761 #include "sleeplock.h"
4762 #include "fs.h"
4763 #include "buf.h"
4764 
4765 #define SECTOR_SIZE   512
4766 #define IDE_BSY       0x80
4767 #define IDE_DRDY      0x40
4768 #define IDE_DF        0x20
4769 #define IDE_ERR       0x01
4770 
4771 #define IDE_CMD_READ  0x20
4772 #define IDE_CMD_WRITE 0x30
4773 #define IDE_CMD_RDMUL 0xc4
4774 #define IDE_CMD_WRMUL 0xc5
4775 
4776 // idequeue points to the buf now being read/written to the disk.
4777 // idequeue->qnext points to the next buf to be processed.
4778 // You must hold idelock while manipulating queue.
4779 
4780 static struct spinlock idelock;
4781 static struct buf *idequeue;
4782 
4783 static int havedisk1;
4784 static void idestart(struct buf*);
4785 
4786 // Wait for IDE disk to become ready.
4787 static int
4788 idewait(int checkerr)
4789 {
4790   int r;
4791 
4792   while(((r = inb(0x1f7)) & (IDE_BSY|IDE_DRDY)) != IDE_DRDY)
4793     ;
4794   if(checkerr && (r & (IDE_DF|IDE_ERR)) != 0)
4795     return -1;
4796   return 0;
4797 }
4798 
4799 
4800 void
4801 ideinit(void)
4802 {
4803   int i;
4804 
4805   initlock(&idelock, "ide");
4806   ioapicenable(IRQ_IDE, ncpu - 1);
4807   idewait(0);
4808 
4809   // Check if disk 1 is present
4810   outb(0x1f6, 0xe0 | (1<<4));
4811   for(i=0; i<1000; i++){
4812     if(inb(0x1f7) != 0){
4813       havedisk1 = 1;
4814       break;
4815     }
4816   }
4817 
4818   // Switch back to disk 0.
4819   outb(0x1f6, 0xe0 | (0<<4));
4820 }
4821 
4822 // Start the request for b.  Caller must hold idelock.
4823 static void
4824 idestart(struct buf *b)
4825 {
4826   if(b == 0)
4827     panic("idestart");
4828   if(b->blockno >= FSSIZE)
4829     panic("incorrect blockno");
4830   int sector_per_block =  BSIZE/SECTOR_SIZE;
4831   int sector = b->blockno * sector_per_block;
4832   int read_cmd = (sector_per_block == 1) ? IDE_CMD_READ :  IDE_CMD_RDMUL;
4833   int write_cmd = (sector_per_block == 1) ? IDE_CMD_WRITE : IDE_CMD_WRMUL;
4834 
4835   if (sector_per_block > 7) panic("idestart");
4836 
4837   idewait(0);
4838   outb(0x3f6, 0);  // generate interrupt
4839   outb(0x1f2, sector_per_block);  // number of sectors
4840   outb(0x1f3, sector & 0xff);
4841   outb(0x1f4, (sector >> 8) & 0xff);
4842   outb(0x1f5, (sector >> 16) & 0xff);
4843   outb(0x1f6, 0xe0 | ((b->dev&1)<<4) | ((sector>>24)&0x0f));
4844   if(b->flags & B_DIRTY){
4845     outb(0x1f7, write_cmd);
4846     outsl(0x1f0, b->data, BSIZE/4);
4847   } else {
4848     outb(0x1f7, read_cmd);
4849   }
4850 }
4851 
4852 // Interrupt handler.
4853 void
4854 ideintr(void)
4855 {
4856   struct buf *b;
4857 
4858   // First queued buffer is the active request.
4859   acquire(&idelock);
4860 
4861   if((b = idequeue) == 0){
4862     release(&idelock);
4863     return;
4864   }
4865   idequeue = b->qnext;
4866 
4867   // Read data if needed.
4868   if(!(b->flags & B_DIRTY) && idewait(1) >= 0)
4869     insl(0x1f0, b->data, BSIZE/4);
4870 
4871   // Wake process waiting for this buf.
4872   b->flags |= B_VALID;
4873   b->flags &= ~B_DIRTY;
4874   wakeup(b);
4875 
4876   // Start disk on next buf in queue.
4877   if(idequeue != 0)
4878     idestart(idequeue);
4879 
4880   release(&idelock);
4881 }
4882 
4883 
4884 
4885 
4886 
4887 
4888 
4889 
4890 
4891 
4892 
4893 
4894 
4895 
4896 
4897 
4898 
4899 
4900 // Sync buf with disk.
4901 // If B_DIRTY is set, write buf to disk, clear B_DIRTY, set B_VALID.
4902 // Else if B_VALID is not set, read buf from disk, set B_VALID.
4903 void
4904 iderw(struct buf *b)
4905 {
4906   struct buf **pp;
4907 
4908   if(!holdingsleep(&b->lock))
4909     panic("iderw: buf not locked");
4910   if((b->flags & (B_VALID|B_DIRTY)) == B_VALID)
4911     panic("iderw: nothing to do");
4912   if(b->dev != 0 && !havedisk1)
4913     panic("iderw: ide disk 1 not present");
4914 
4915   acquire(&idelock);  //DOC:acquire-lock
4916 
4917   // Append b to idequeue.
4918   b->qnext = 0;
4919   for(pp=&idequeue; *pp; pp=&(*pp)->qnext)  //DOC:insert-queue
4920     ;
4921   *pp = b;
4922 
4923   // Start disk if necessary.
4924   if(idequeue == b)
4925     idestart(b);
4926 
4927   // Wait for request to finish.
4928   while((b->flags & (B_VALID|B_DIRTY)) != B_VALID){
4929     sleep(b, &idelock);
4930   }
4931 
4932 
4933   release(&idelock);
4934 }
4935 
4936 
4937 
4938 
4939 
4940 
4941 
4942 
4943 
4944 
4945 
4946 
4947 
4948 
4949 
