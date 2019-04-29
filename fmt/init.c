9050 // init: The initial user-level program
9051 //#include "proc.h"
9052 #include "types.h"
9053 #include "stat.h"
9054 #include "user.h"
9055 #include "fcntl.h"
9056 
9057 char *argv[] = { "sh", 0 };
9058 
9059 int
9060 main(void)
9061 {
9062   int pid, wpid;
9063 
9064   if(open("console", O_RDWR) < 0){
9065     mknod("console", 1, 1);
9066     open("console", O_RDWR);
9067   }
9068   dup(0);  // stdout
9069   dup(0);  // stderr
9070   for(;;){
9071     printf(1, "init: starting sh\n");
9072 	printf(1, "Student ID: 2016312029\n");
9073 	printf(1, "Name: Hyeongjun Park\n");
9074     pid = fork();
9075     if(pid < 0){
9076       printf(1, "init: fork failed\n");
9077       exit();
9078     }
9079     if(pid == 0){
9080       exec("sh", argv);
9081       printf(1, "init: exec sh failed\n");
9082       exit();
9083     }
9084     while((wpid=wait()) >= 0 && wpid != pid)
9085       printf(1, "zombie!\n");
9086   }
9087 }
9088 
9089 
9090 
9091 
9092 
9093 
9094 
9095 
9096 
9097 
9098 
9099 
