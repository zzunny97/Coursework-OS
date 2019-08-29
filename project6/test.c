#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
	int before, after;
	int pid;

	printf(1, "TEST: ");
	
	before = numfree();

	
	pid = fork();
	if(pid == 0){
		after = numfree();
		if(before - after == 68)
			printf(1, "OK\n");
		else
			printf(1, "WRONG\n");
    //printf(1, "result: %d\n", before - after);
		exit();
	}
	else{
		//printf(1, "parent waiting..\n");
		wait();
		//printf(1, "wait complete\n");
	}

	exit();
}
