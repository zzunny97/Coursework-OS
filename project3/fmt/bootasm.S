9650 #include "asm.h"
9651 #include "memlayout.h"
9652 #include "mmu.h"
9653 
9654 # Start the first CPU: switch to 32-bit protected mode, jump into C.
9655 # The BIOS loads this code from the first sector of the hard disk into
9656 # memory at physical address 0x7c00 and starts executing in real mode
9657 # with %cs=0 %ip=7c00.
9658 
9659 .code16                       # Assemble for 16-bit mode
9660 .globl start
9661 start:
9662   cli                         # BIOS enabled interrupts; disable
9663 
9664   # Zero data segment registers DS, ES, and SS.
9665   xorw    %ax,%ax             # Set %ax to zero
9666   movw    %ax,%ds             # -> Data Segment
9667   movw    %ax,%es             # -> Extra Segment
9668   movw    %ax,%ss             # -> Stack Segment
9669 
9670   # Physical address line A20 is tied to zero so that the first PCs
9671   # with 2 MB would run software that assumed 1 MB.  Undo that.
9672 seta20.1:
9673   inb     $0x64,%al               # Wait for not busy
9674   testb   $0x2,%al
9675   jnz     seta20.1
9676 
9677   movb    $0xd1,%al               # 0xd1 -> port 0x64
9678   outb    %al,$0x64
9679 
9680 seta20.2:
9681   inb     $0x64,%al               # Wait for not busy
9682   testb   $0x2,%al
9683   jnz     seta20.2
9684 
9685   movb    $0xdf,%al               # 0xdf -> port 0x60
9686   outb    %al,$0x60
9687 
9688   # Switch from real to protected mode.  Use a bootstrap GDT that makes
9689   # virtual addresses map directly to physical addresses so that the
9690   # effective memory map doesn't change during the transition.
9691   lgdt    gdtdesc
9692   movl    %cr0, %eax
9693   orl     $CR0_PE, %eax
9694   movl    %eax, %cr0
9695 
9696 
9697 
9698 
9699 
9700   # Complete the transition to 32-bit protected mode by using a long jmp
9701   # to reload %cs and %eip.  The segment descriptors are set up with no
9702   # translation, so that the mapping is still the identity mapping.
9703   ljmp    $(SEG_KCODE<<3), $start32
9704 
9705 .code32  # Tell assembler to generate 32-bit code now.
9706 start32:
9707   # Set up the protected-mode data segment registers
9708   movw    $(SEG_KDATA<<3), %ax    # Our data segment selector
9709   movw    %ax, %ds                # -> DS: Data Segment
9710   movw    %ax, %es                # -> ES: Extra Segment
9711   movw    %ax, %ss                # -> SS: Stack Segment
9712   movw    $0, %ax                 # Zero segments not ready for use
9713   movw    %ax, %fs                # -> FS
9714   movw    %ax, %gs                # -> GS
9715 
9716   # Set up the stack pointer and call into C.
9717   movl    $start, %esp
9718   call    bootmain
9719 
9720   # If bootmain returns (it shouldn't), trigger a Bochs
9721   # breakpoint if running under Bochs, then loop.
9722   movw    $0x8a00, %ax            # 0x8a00 -> port 0x8a00
9723   movw    %ax, %dx
9724   outw    %ax, %dx
9725   movw    $0x8ae0, %ax            # 0x8ae0 -> port 0x8a00
9726   outw    %ax, %dx
9727 spin:
9728   jmp     spin
9729 
9730 # Bootstrap GDT
9731 .p2align 2                                # force 4 byte alignment
9732 gdt:
9733   SEG_NULLASM                             # null seg
9734   SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # code seg
9735   SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg
9736 
9737 gdtdesc:
9738   .word   (gdtdesc - gdt - 1)             # sizeof(gdt) - 1
9739   .long   gdt                             # address gdt
9740 
9741 
9742 
9743 
9744 
9745 
9746 
9747 
9748 
9749 
