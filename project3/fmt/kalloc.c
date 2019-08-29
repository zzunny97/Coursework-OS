3450 // Physical memory allocator, intended to allocate
3451 // memory for user processes, kernel stacks, page table pages,
3452 // and pipe buffers. Allocates 4096-byte pages.
3453 
3454 #include "types.h"
3455 #include "defs.h"
3456 #include "param.h"
3457 #include "memlayout.h"
3458 #include "mmu.h"
3459 #include "spinlock.h"
3460 
3461 void freerange(void *vstart, void *vend);
3462 extern char end[]; // first address after kernel loaded from ELF file
3463                    // defined by the kernel linker script in kernel.ld
3464 
3465 struct run {
3466   struct run *next;
3467 };
3468 
3469 struct {
3470   struct spinlock lock;
3471   int use_lock;
3472   struct run *freelist;
3473 } kmem;
3474 
3475 // Initialization happens in two phases.
3476 // 1. main() calls kinit1() while still using entrypgdir to place just
3477 // the pages mapped by entrypgdir on free list.
3478 // 2. main() calls kinit2() with the rest of the physical pages
3479 // after installing a full page table that maps them on all cores.
3480 void
3481 kinit1(void *vstart, void *vend)
3482 {
3483   initlock(&kmem.lock, "kmem");
3484   kmem.use_lock = 0;
3485   freerange(vstart, vend);
3486 }
3487 
3488 void
3489 kinit2(void *vstart, void *vend)
3490 {
3491   freerange(vstart, vend);
3492   kmem.use_lock = 1;
3493 }
3494 
3495 
3496 
3497 
3498 
3499 
3500 void
3501 freerange(void *vstart, void *vend)
3502 {
3503   char *p;
3504   p = (char*)PGROUNDUP((uint)vstart);
3505   for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
3506     kfree(p);
3507 }
3508 
3509 // Free the page of physical memory pointed at by v,
3510 // which normally should have been returned by a
3511 // call to kalloc().  (The exception is when
3512 // initializing the allocator; see kinit above.)
3513 void
3514 kfree(char *v)
3515 {
3516   struct run *r;
3517 
3518   if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
3519     panic("kfree");
3520 
3521   // Fill with junk to catch dangling refs.
3522   memset(v, 1, PGSIZE);
3523 
3524   if(kmem.use_lock)
3525     acquire(&kmem.lock);
3526   r = (struct run*)v;
3527   r->next = kmem.freelist;
3528   kmem.freelist = r;
3529   if(kmem.use_lock)
3530     release(&kmem.lock);
3531 }
3532 
3533 // Allocate one 4096-byte page of physical memory.
3534 // Returns a pointer that the kernel can use.
3535 // Returns 0 if the memory cannot be allocated.
3536 char*
3537 kalloc(void)
3538 {
3539   struct run *r;
3540 
3541   if(kmem.use_lock)
3542     acquire(&kmem.lock);
3543   r = kmem.freelist;
3544   if(r)
3545     kmem.freelist = r->next;
3546   if(kmem.use_lock)
3547     release(&kmem.lock);
3548   return (char*)r;
3549 }
