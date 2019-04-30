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
  cprintf("func: mutex_init\n");
  if(mutex->valid == 1)
    return -2;

  mutex->valid = 1;
  initlock(&mutex->lock, (char*)mutex);
  mutex->current = NULL;
  for(int i=0; i<NTHREAD; i++)
    mutex->queue[i] = NULL;
  mutex->qsize = 0;
  mutex->qnext = 0;
  return 0;
}

int mutex_lock(struct mutex_t *mutex)
{

  cprintf("func: mutex_lock\n");
  struct proc *curproc = myproc();
  if(mutex->valid != 1)
    return -2;
  if(mutex->current == curproc)
    return -3;

  
  if(mutex->current == NULL) {
    mutex->current =curproc;
  }
  else {
    mutex->queue[mutex->qsize++] = curproc;
    //sleep(curproc, &(mutex->lock));
    while(mutex->current->tid != curproc->tid);
  }
  acquire(&(mutex->lock));
  return 0;
}

int mutex_unlock(struct mutex_t *mutex)
{

  cprintf("func: mutex_unlock\n");
  struct proc *curproc = myproc();
  if(mutex->valid != 1)
    return -2;
  if(mutex->current != curproc)
    return -3;

  //cprintf("abcde\n");
  /*if(!holding(&mutex->lock)){
    cprintf("not holding mutex in mutex_unlock\n");
    return 0;

  }*/
  release(&mutex->lock);
  //cprintf("qwerr\n");
  mutex->current = NULL;
  //cprintf("hello\n");
  if(mutex->qsize != 0){
  //cprintf("hello\n");
    mutex->queue[mutex->qnext++] = mutex->current;
  }
  //cprintf("hello\n");
  /*
  if(mutex->qsize != 0) {
    mutex->current = mutex->queue[mutex->qnext];
    mutex->queue[mutex->qnext]->state = RUNNABLE;
    mutex->qnext++;
    acquire(&mutex->lock);
  }*/
  return 0;
}

int cond_init(struct cond_t *cond)
{

  cprintf("func: cond init\n");
  if(cond->valid == 1)
    return -2;

  cond->valid = 1;
  cond->active = 0;
  initlock(&cond->lock, (char*)cond);
  for(int i=0; i<NTHREAD; i++)
    cond->queue[i] = NULL;
  cond->qsize = 0;
  cond->qnext = 0;
  return 0;
}

int cond_wait(struct cond_t *cond, struct mutex_t *mutex)
{

  cprintf("func: cond_wait\n");
  struct proc *curproc = myproc();
  if(cond->valid != 1 || mutex->valid != 1)
    return -2;
  if(!holding(&mutex->lock))
    return -3;
  if(mutex->current != curproc)
    return -3;



  if(!holding(&mutex->lock)) {
    cprintf("not holding mutex\n");
    return 0;
  }

  //mutex_unlock(mutex);
  //cond->queue[cond->qsize++] = curproc;
  //sleep(curproc, &(cond->lock));

 /* 
  for(int i=0; i<cond->qsize; i++) {
    if(cond->queue[i]->tid == curproc->tid)
      return 0;
  }*/
  
  if(mutex->current == NULL) {
    mutex->current =curproc;
  }
  else {
    mutex->queue[mutex->qsize++] = curproc;
    //sleep(curproc, &(mutex->lock));
    while(mutex->current->tid != curproc->tid);
  }
  acquire(&(mutex->lock));




  return 0;
  cond->queue[cond->qsize++] = curproc;
  cprintf("tid: %dstart sleeping\n", curproc->tid);
  while(cond->qnext->tid != curproc->tid);
  //sleep(curproc, &(mutex->lock));
  /*
  if(cond->current != NULL){
    cond->queue[cond->qsize] = curproc;
    cond->qsize++;
   // sleep(curproc, &(cond->lock));
    //cprintf("sleep start\n");
    //mutex_lock(mutex);
  }
  else{
    cond->current = curproc;
    mutex_lock(mutex);
  }
  //mutex_lock(mutex);*/
  return 0;
}

int cond_signal(struct cond_t *cond)
{

  cprintf("func: cond_signal\n");
  struct proc *curproc = myproc();
  if(cond->valid != 1)
    return -2;
  
  if(cond->queue[cond->qnext] != NULL) {
    wakeup(cond->queue[cond->qnext]);
    cond->qnext++;
    //wakeup(cond->current);
  }
  
  return 0;  
}
