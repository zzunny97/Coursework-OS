3900 #include "types.h"
3901 #include "defs.h"
3902 #include "param.h"
3903 #include "memlayout.h"
3904 #include "mmu.h"
3905 #include "proc.h"
3906 #include "x86.h"
3907 #include "syscall.h"
3908 
3909 // User code makes a system call with INT T_SYSCALL.
3910 // System call number in %eax.
3911 // Arguments on the stack, from the user call to the C
3912 // library system call function. The saved user %esp points
3913 // to a saved program counter, and then the first argument.
3914 
3915 // Fetch the int at addr from the current process.
3916 int
3917 fetchint(uint addr, int *ip)
3918 {
3919   struct proc *curproc = myproc();
3920 
3921   if(addr >= curproc->sz || addr+4 > curproc->sz)
3922     return -1;
3923   *ip = *(int*)(addr);
3924   return 0;
3925 }
3926 
3927 // Fetch the nul-terminated string at addr from the current process.
3928 // Doesn't actually copy the string - just sets *pp to point at it.
3929 // Returns length of string, not including nul.
3930 int
3931 fetchstr(uint addr, char **pp)
3932 {
3933   char *s, *ep;
3934   struct proc *curproc = myproc();
3935 
3936   if(addr >= curproc->sz)
3937     return -1;
3938   *pp = (char*)addr;
3939   ep = (char*)curproc->sz;
3940   for(s = *pp; s < ep; s++){
3941     if(*s == 0)
3942       return s - *pp;
3943   }
3944   return -1;
3945 }
3946 
3947 
3948 
3949 
3950 // Fetch the nth 32-bit system call argument.
3951 int
3952 argint(int n, int *ip)
3953 {
3954   return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
3955 }
3956 
3957 // Fetch the nth word-sized system call argument as a pointer
3958 // to a block of memory of size bytes.  Check that the pointer
3959 // lies within the process address space.
3960 int
3961 argptr(int n, char **pp, int size)
3962 {
3963   int i;
3964   struct proc *curproc = myproc();
3965 
3966   if(argint(n, &i) < 0){
3967     cprintf("err1\n");
3968     return -1;
3969   }
3970   if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz){
3971     cprintf("err2\n");
3972     return -1;
3973   }
3974   *pp = (char*)i;
3975   return 0;
3976 }
3977 
3978 // Fetch the nth word-sized system call argument as a string pointer.
3979 // Check that the pointer is valid and the string is nul-terminated.
3980 // (There is no shared writable memory, so the string can't change
3981 // between this check and being used by the kernel.)
3982 int
3983 argstr(int n, char **pp)
3984 {
3985   int addr;
3986   if(argint(n, &addr) < 0)
3987     return -1;
3988   return fetchstr(addr, pp);
3989 }
3990 
3991 
3992 
3993 
3994 
3995 
3996 
3997 
3998 
3999 
4000 extern int sys_chdir(void);
4001 extern int sys_close(void);
4002 extern int sys_dup(void);
4003 extern int sys_exec(void);
4004 extern int sys_exit(void);
4005 extern int sys_fork(void);
4006 extern int sys_fstat(void);
4007 extern int sys_getpid(void);
4008 extern int sys_kill(void);
4009 extern int sys_link(void);
4010 extern int sys_mkdir(void);
4011 extern int sys_mknod(void);
4012 extern int sys_open(void);
4013 extern int sys_pipe(void);
4014 extern int sys_read(void);
4015 extern int sys_sbrk(void);
4016 extern int sys_sleep(void);
4017 extern int sys_unlink(void);
4018 extern int sys_wait(void);
4019 extern int sys_write(void);
4020 extern int sys_uptime(void);
4021 extern int sys_halt(void);
4022 extern int sys_getnice(void);
4023 extern int sys_setnice(void);
4024 extern int sys_ps(void);
4025 extern int sys_thread_create(void);
4026 extern int sys_thread_exit(void);
4027 extern int sys_thread_join(void);
4028 extern int sys_gettid(void);
4029 extern int sys_mutex_init(void);
4030 extern int sys_mutex_lock(void);
4031 extern int sys_mutex_unlock(void);
4032 extern int sys_cond_init(void);
4033 extern int sys_cond_wait(void);
4034 extern int sys_cond_signla(void);
4035 
4036 static int (*syscalls[])(void) = {
4037 [SYS_fork]    sys_fork,
4038 [SYS_exit]    sys_exit,
4039 [SYS_wait]    sys_wait,
4040 [SYS_pipe]    sys_pipe,
4041 [SYS_read]    sys_read,
4042 [SYS_kill]    sys_kill,
4043 [SYS_exec]    sys_exec,
4044 [SYS_fstat]   sys_fstat,
4045 [SYS_chdir]   sys_chdir,
4046 [SYS_dup]     sys_dup,
4047 [SYS_getpid]  sys_getpid,
4048 [SYS_sbrk]    sys_sbrk,
4049 [SYS_sleep]   sys_sleep,
4050 [SYS_uptime]  sys_uptime,
4051 [SYS_open]    sys_open,
4052 [SYS_write]   sys_write,
4053 [SYS_mknod]   sys_mknod,
4054 [SYS_unlink]  sys_unlink,
4055 [SYS_link]    sys_link,
4056 [SYS_mkdir]   sys_mkdir,
4057 [SYS_close]   sys_close,
4058 [SYS_halt]    sys_halt,
4059 [SYS_getnice] sys_getnice,
4060 [SYS_setnice] sys_setnice,
4061 [SYS_ps]      sys_ps,
4062 [SYS_thread_create]      sys_thread_create,
4063 [SYS_thread_exit]      sys_thread_exit,
4064 [SYS_thread_join]      sys_thread_join,
4065 [SYS_gettid]      sys_gettid,
4066 [SYS_mutex_init] sys_mutex_init,
4067 [SYS_mutex_lock] sys_mutex_lock,
4068 [SYS_mutex_unlock] sys_mutex_unlock,
4069 [SYS_cond_init] sys_cond_init,
4070 [SYS_cond_wait] sys_cond_wait,
4071 [SYS_cond_signal] sys_cond_signal,
4072 };
4073 
4074 
4075 void
4076 syscall(void)
4077 {
4078   int num;
4079   struct proc *curproc = myproc();
4080 
4081   num = curproc->tf->eax;
4082   if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
4083     curproc->tf->eax = syscalls[num]();
4084   } else {
4085     cprintf("%d %s: unknown sys call %d\n",
4086             curproc->pid, curproc->name, num);
4087     curproc->tf->eax = -1;
4088   }
4089 }
4090 
4091 
4092 
4093 
4094 
4095 
4096 
4097 
4098 
4099 
