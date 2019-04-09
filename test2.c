#include "types.h"
#include "stat.h"
#include "user.h"
void *
thread(void *arg)
{
  int i;
  int sum = 0;
  for(i=0; i<10000000; i++)
    sum += i;
	thread_exit((void *)0x87654321);
}

int
main(int argc, char **argv)
{
	void *stack;
	void *retval;
	int tid;

	printf(1, "TEST: ");

	stack = malloc(4096);
  void* stack2 = malloc(4096);
	
	tid = thread_create(thread, (void *)0x12345678, stack);
  int tid2 = thread_create(thread, (void*)0x234567890, stack2);
	if(tid == -1) {
		printf(1, "1\n");
    printf(1, "WRONG\n");
		exit();
	}
  if(tid2 == -1){
    printf(1, "1-1\n");
    printf(1, "wrong\n");
  }
  printf(1, "before thread_join\n");
	if(thread_join(tid, &retval) == -1) {
    printf(1, "2\n");
		printf(1, "WRONG\n");
		exit();
	}
  void* retval2;
  if(thread_join(tid2, &retval2) == -2){
    printf(1, "2-1\n");
    printf(1, "wrong\n");
  }

	if(retval != (void *)0x87654321) {
    printf(1, "3\n");
		printf(1, "WRONG\n");
		exit();
	}
  if(retval2 != (void*)0x87654321){
    printf(1, "3-1\n");
    printf(1, "wrong\n");
    exit();
  }

	free(stack);
  free(stack2);
	printf(1, "OK\n");

	exit();
}

