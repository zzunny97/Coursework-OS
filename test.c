#include "types.h"
#include "stat.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"
#include "user.h"
#include "synch.h"

#define NTHREAD 8 

void *stack[NTHREAD];
int tid[NTHREAD];
void *retval[NTHREAD];

struct mutex_t m;
struct cond_t c;
int buffer[10];
int idx=0;
int condition = 0;

void *thread(void *arg){
  printf(1, "19191919191919191919\n");
  mutex_lock(&m);
  printf(1, "************\n");
  while(condition == 0){
    printf(1, "----------------cond_wait while loop--------------\n");
    cond_wait(&c, &m);
  }
  printf(1, "cond_wait complete\n");


  condition--;
  buffer[idx++] = (int)arg;
  printf(1, "arg: %d\t buffer[idx]: %d\n", (int)arg, buffer[idx-1]);
  mutex_unlock(&m);
  printf(1, "thread_exit\n\n");
  thread_exit(0);
}

  int
main(int argc, char **argv)
{
  int i;

  printf(1, "TEST: ");

  for(i=0;i<NTHREAD;i++)
    stack[i] = malloc(4096);
  mutex_init(&m);
  cond_init(&c);
  printf(1, "\nmutex and cv init complete\n");
  for(i=0;i<NTHREAD-1;i++){
    tid[i] = thread_create(thread, (void *)i, stack[i]);
    printf(1, "tid[%d]: %d\n", i, tid[i]);
    if(tid[i] == -1){
      printf(1, "thread create failed\n");
      exit();
    }
  }
  printf(1, "real process going cond_signal\n");
  for(i=0;i<NTHREAD-1;i++){
    mutex_lock(&m);
    condition++;
    printf(1, "condition signal!!!!\n");
    cond_signal(&c);
    mutex_unlock(&m);
  }

  printf(1, "real process going thread_join\n");
  for(i=0;i<NTHREAD-1;i++){
    if(thread_join(tid[i], &retval[i]) == -1){
      printf(1, "thread join failed\n");
      exit();
    }
    printf(1, "thread join!\n");
  }
  printf(1, "thread_join complete\n");

  for(i=0;i<NTHREAD-1;i++){
    printf(1, "buffer[%d] = %d\n",i, buffer[i]);
    if(buffer[i] != i){
      printf(1, "WRONG\n");
      exit();
    }
  }

  for(i=0;i<NTHREAD;i++)
    free(stack[i]);

  printf(1, "OK\n");

  exit();
}
