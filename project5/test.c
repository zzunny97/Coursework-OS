#include "types.h"
#include "stat.h"
#include "user.h"


void test(int n){
 	//printf(1, "n: %d\n", n);
	if(n > 0){
		test(n - 1);
	}
	exit();
}

int main(int argc, char *argv[]){
	int pid;

	printf(1, "================================== Result=================================\n");

	pid = fork();
	
	if(pid == 0)	{
		test(1024); 
		printf(1, "Fail\n");
	}
	else	wait();

	printf(1, "==========================================================================\n");


	exit();	
}
