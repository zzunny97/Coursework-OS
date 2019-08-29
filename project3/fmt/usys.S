9000 #include "syscall.h"
9001 #include "traps.h"
9002 
9003 #define SYSCALL(name) \
9004   .globl name; \
9005   name: \
9006     movl $SYS_ ## name, %eax; \
9007     int $T_SYSCALL; \
9008     ret
9009 
9010 SYSCALL(fork)
9011 SYSCALL(exit)
9012 SYSCALL(wait)
9013 SYSCALL(pipe)
9014 SYSCALL(read)
9015 SYSCALL(write)
9016 SYSCALL(close)
9017 SYSCALL(kill)
9018 SYSCALL(exec)
9019 SYSCALL(open)
9020 SYSCALL(mknod)
9021 SYSCALL(unlink)
9022 SYSCALL(fstat)
9023 SYSCALL(link)
9024 SYSCALL(mkdir)
9025 SYSCALL(chdir)
9026 SYSCALL(dup)
9027 SYSCALL(getpid)
9028 SYSCALL(sbrk)
9029 SYSCALL(sleep)
9030 SYSCALL(uptime)
9031 SYSCALL(halt)
9032 SYSCALL(getnice)
9033 SYSCALL(setnice)
9034 SYSCALL(ps)
9035 SYSCALL(thread_create)
9036 SYSCALL(thread_exit)
9037 SYSCALL(thread_join)
9038 SYSCALL(gettid)
9039 SYSCALL(mutex_init)
9040 SYSCALL(mutex_lock)
9041 SYSCALL(mutex_unlock)
9042 SYSCALL(cond_init)
9043 SYSCALL(cond_wait)
9044 SYSCALL(cond_signal)
9045 
9046 
9047 
9048 
9049 
