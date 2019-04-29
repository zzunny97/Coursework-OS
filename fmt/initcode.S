8950 # Initial process execs /init.
8951 # This code runs in user space.
8952 
8953 #include "syscall.h"
8954 #include "traps.h"
8955 
8956 
8957 # exec(init, argv)
8958 .globl start
8959 start:
8960   pushl $argv
8961   pushl $init
8962   pushl $0  // where caller pc would be
8963   movl $SYS_exec, %eax
8964   int $T_SYSCALL
8965 
8966 # for(;;) exit();
8967 exit:
8968   movl $SYS_exit, %eax
8969   int $T_SYSCALL
8970   jmp exit
8971 
8972 # char init[] = "/init\0";
8973 init:
8974   .string "/init\0"
8975 
8976 # char *argv[] = { init, 0 };
8977 .p2align 2
8978 argv:
8979   .long init
8980   .long 0
8981 
8982 
8983 
8984 
8985 
8986 
8987 
8988 
8989 
8990 
8991 
8992 
8993 
8994 
8995 
8996 
8997 
8998 
8999 
