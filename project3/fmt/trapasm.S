3650 #include "mmu.h"
3651 
3652   # vectors.S sends all traps here.
3653 .globl alltraps
3654 alltraps:
3655   # Build trap frame.
3656   pushl %ds
3657   pushl %es
3658   pushl %fs
3659   pushl %gs
3660   pushal
3661 
3662   # Set up data segments.
3663   movw $(SEG_KDATA<<3), %ax
3664   movw %ax, %ds
3665   movw %ax, %es
3666 
3667   # Call trap(tf), where tf=%esp
3668   pushl %esp
3669   call trap
3670   addl $4, %esp
3671 
3672   # Return falls through to trapret...
3673 .globl trapret
3674 trapret:
3675   popal
3676   popl %gs
3677   popl %fs
3678   popl %es
3679   popl %ds
3680   addl $0x8, %esp  # trapno and errcode
3681   iret
3682 
3683 
3684 
3685 
3686 
3687 
3688 
3689 
3690 
3691 
3692 
3693 
3694 
3695 
3696 
3697 
3698 
3699 
