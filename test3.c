#include "types.h"
#include "stat.h"
#include "user.h"
void *
thread(void *arg)
{
  if(gettid() == 1){
    printf(1, "this is thread 1!\n");
  }
  else if(gettid()==2){
    printf(1, "this is thread2!\n");
  }
  else if(gettid()==3){
    printf(1, "this is thread3!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread4!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread5!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread6!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread7!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread8!\n");
  }
  else if(gettid()==4){
    printf(1, "this is thread9!\n");
  }
  else{ 
    printf(1, "what the hell!\n");
  }
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
  void* stack3 = malloc(4096);
  void* stack4 = malloc(4096);
  void* stack5 = malloc(4096);
  void* stack6 = malloc(4096);
  void* stack7 = malloc(4096);
  void* stack8 = malloc(4096);
  void* stack9 = malloc(4096);
	
	tid = thread_create(thread, (void *)0x12345678, stack);
  int tid2 = thread_create(thread, (void*)0x234567890, stack2);
  int tid3 = thread_create(thread, (void*)0x234567890, stack3);
  int tid4 = thread_create(thread, (void*)0x234567890, stack4);
  int tid5 = thread_create(thread, (void*)0x234567890, stack5);
  int tid6 = thread_create(thread, (void*)0x234567890, stack6);
  int tid7 = thread_create(thread, (void*)0x234567890, stack7);
  int tid8 = thread_create(thread, (void*)0x234567890, stack8);
  int tid9 = thread_create(thread, (void*)0x234567890, stack9);
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

