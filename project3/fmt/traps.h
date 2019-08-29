3550 // x86 trap and interrupt constants.
3551 
3552 // Processor-defined:
3553 #define T_DIVIDE         0      // divide error
3554 #define T_DEBUG          1      // debug exception
3555 #define T_NMI            2      // non-maskable interrupt
3556 #define T_BRKPT          3      // breakpoint
3557 #define T_OFLOW          4      // overflow
3558 #define T_BOUND          5      // bounds check
3559 #define T_ILLOP          6      // illegal opcode
3560 #define T_DEVICE         7      // device not available
3561 #define T_DBLFLT         8      // double fault
3562 // #define T_COPROC      9      // reserved (not used since 486)
3563 #define T_TSS           10      // invalid task switch segment
3564 #define T_SEGNP         11      // segment not present
3565 #define T_STACK         12      // stack exception
3566 #define T_GPFLT         13      // general protection fault
3567 #define T_PGFLT         14      // page fault
3568 // #define T_RES        15      // reserved
3569 #define T_FPERR         16      // floating point error
3570 #define T_ALIGN         17      // aligment check
3571 #define T_MCHK          18      // machine check
3572 #define T_SIMDERR       19      // SIMD floating point error
3573 
3574 // These are arbitrarily chosen, but with care not to overlap
3575 // processor defined exceptions or interrupt vectors.
3576 #define T_SYSCALL       64      // system call
3577 #define T_DEFAULT      500      // catchall
3578 
3579 #define T_IRQ0          32      // IRQ 0 corresponds to int T_IRQ
3580 
3581 #define IRQ_TIMER        0
3582 #define IRQ_KBD          1
3583 #define IRQ_COM1         4
3584 #define IRQ_IDE         14
3585 #define IRQ_ERROR       19
3586 #define IRQ_SPURIOUS    31
3587 
3588 
3589 
3590 
3591 
3592 
3593 
3594 
3595 
3596 
3597 
3598 
3599 
