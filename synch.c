#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "stat.h"
#include "fcntl.h"
#include "synch.h"


#define NULL 0



int mutex_init(struct mutex_t *mutex)
{
  if(mutex->valid == 1)
    return -2;
  mutex->valid = 1;
  mutex->current = NULL;
  for(int i=0; i<NTHREAD; i++)
    mutex->queue[i] = NULL;
  mutex->qsize = 0;
  mutex->qnext = 0;
  return 0;
}

int mutex_lock(struct mutex_t *mutex)
{
  struct proc *curproc = myproc();

  if(mutex->valid != 1)
    return -2;
  if(mutex->current == curproc)
    return -3;


  while(xchg(&mutex->lock, 1) != 0);
  if(mutex->current == NULL) {
    mutex->current = curproc;
  }
  else {
    mutex->queue[mutex->qsize] = curproc;
    mutex->qsize++;
    while(mutex->current != NULL){
    }
    while(mutex->current->tid != curproc->tid){
    }
  }
  return 0;
}

int mutex_unlock(struct mutex_t *mutex)
{

  struct proc *curproc = myproc();
  if(mutex->valid != 1)
    return -2;
  if(mutex->current != curproc)
    return -3;

  if(mutex->queue[mutex->qnext] != NULL){
    mutex->current = mutex->queue[mutex->qnext];
    mutex->qnext++;
  }
  xchg(&mutex->lock, 0);
  mutex->current = NULL;
  return 0;
}

int cond_init(struct cond_t *cond)
{
  if(cond->valid == 1)
    return -2;

  cond->valid = 1;
  cond->current = NULL;
  for(int i=0; i<NTHREAD; i++)
    cond->queue[i] = NULL;
  cond->qsize = 0;
  cond->qnext = 0;
  return 0;
}

int cond_wait(struct cond_t *cond, struct mutex_t *mutex)
{
  struct proc *curproc = myproc();
  if(cond->valid != 1 || mutex->valid != 1){
    return -2;
  }

  if(mutex->current != curproc){
    return -3;
  }
  mutex_unlock(mutex);

  if(cond->current == NULL){
    cond->current = curproc;
  }

  else{
    cond->queue[cond->qsize] = curproc;
    cond->qsize++;
  }

  tsleep(curproc);
  mutex_lock(mutex);
  return 0;
}

int cond_signal(struct cond_t *cond)
{
  struct proc *curproc = myproc();
  if(cond->valid != 1){
    return -2;
  }
  
  if(cond->current != NULL) {
    twake(cond->current);
    cond->current = NULL;
    return 0;
  }


  else if(cond->queue[cond->qnext] != NULL) {
    twake(cond->queue[cond->qnext]);
    cond->qnext++;
    cond->current = NULL;
  }

  return 0;  
}
