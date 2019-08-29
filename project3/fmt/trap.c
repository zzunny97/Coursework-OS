3700 #include "types.h"
3701 #include "defs.h"
3702 #include "param.h"
3703 #include "memlayout.h"
3704 #include "mmu.h"
3705 #include "proc.h"
3706 #include "x86.h"
3707 #include "traps.h"
3708 #include "spinlock.h"
3709 
3710 // Interrupt descriptor table (shared by all CPUs).
3711 struct gatedesc idt[256];
3712 extern uint vectors[];  // in vectors.S: array of 256 entry pointers
3713 struct spinlock tickslock;
3714 uint ticks;
3715 
3716 void
3717 tvinit(void)
3718 {
3719   int i;
3720 
3721   for(i = 0; i < 256; i++)
3722     SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
3723   SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
3724 
3725   initlock(&tickslock, "time");
3726 }
3727 
3728 void
3729 idtinit(void)
3730 {
3731   lidt(idt, sizeof(idt));
3732 }
3733 
3734 
3735 
3736 
3737 
3738 
3739 
3740 
3741 
3742 
3743 
3744 
3745 
3746 
3747 
3748 
3749 
3750 void
3751 trap(struct trapframe *tf)
3752 {
3753   if(tf->trapno == T_SYSCALL){
3754     if(myproc()->killed)
3755       exit();
3756     myproc()->tf = tf;
3757     syscall();
3758     if(myproc()->killed)
3759       exit();
3760     return;
3761   }
3762 
3763   switch(tf->trapno){
3764   case T_IRQ0 + IRQ_TIMER:
3765     if(cpuid() == 0){
3766       acquire(&tickslock);
3767       ticks++;
3768       wakeup(&ticks);
3769       release(&tickslock);
3770     }
3771     lapiceoi();
3772     break;
3773   case T_IRQ0 + IRQ_IDE:
3774     ideintr();
3775     lapiceoi();
3776     break;
3777   case T_IRQ0 + IRQ_IDE+1:
3778     // Bochs generates spurious IDE1 interrupts.
3779     break;
3780   case T_IRQ0 + IRQ_KBD:
3781     kbdintr();
3782     lapiceoi();
3783     break;
3784   case T_IRQ0 + IRQ_COM1:
3785     uartintr();
3786     lapiceoi();
3787     break;
3788   case T_IRQ0 + 7:
3789   case T_IRQ0 + IRQ_SPURIOUS:
3790     cprintf("cpu%d: spurious interrupt at %x:%x\n",
3791             cpuid(), tf->cs, tf->eip);
3792     lapiceoi();
3793     break;
3794 
3795 
3796 
3797 
3798 
3799 
3800   default:
3801     if(myproc() == 0 || (tf->cs&3) == 0){
3802       // In kernel, it must be our mistake.
3803       cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
3804               tf->trapno, cpuid(), tf->eip, rcr2());
3805       panic("trap");
3806     }
3807     // In user space, assume process misbehaved.
3808     cprintf("pid %d %s: trap %d err %d on cpu %d "
3809             "eip 0x%x addr 0x%x--kill proc\n",
3810             myproc()->pid, myproc()->name, tf->trapno,
3811             tf->err, cpuid(), tf->eip, rcr2());
3812     myproc()->killed = 1;
3813   }
3814 
3815   // Force process exit if it has been killed and is in user space.
3816   // (If it is still executing in the kernel, let it keep running
3817   // until it gets to the regular system call return.)
3818   if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
3819     exit();
3820 
3821   // Force process to give up CPU on clock tick.
3822   // If interrupts were on while locks held, would need to check nlock.
3823   if(myproc() && myproc()->state == RUNNING &&
3824      tf->trapno == T_IRQ0+IRQ_TIMER)
3825     yield();
3826 
3827   // Check if the process has been killed since we yielded
3828   if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
3829     exit();
3830 }
3831 
3832 
3833 
3834 
3835 
3836 
3837 
3838 
3839 
3840 
3841 
3842 
3843 
3844 
3845 
3846 
3847 
3848 
3849 
