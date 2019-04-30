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
  //initlock(&mutex->lock, (char*)mutex);
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
  cprintf("pid: %d\t", curproc->pid);
  cprintf("tid: %d\t", curproc->tid);
  cprintf("mutex->qsize: %d\t", mutex->qsize);
  
  if(mutex->valid != 1)
    return -2;
  if(mutex->current == curproc)
    return -3;

  
  while(xchg(&mutex->lock, 1) != 0);
  if(mutex->current == NULL) {
    //cprintf("if statement\n");
    mutex->current = curproc;
    if(mutex->current == NULL)
      cprintf("if statement error\n");
  }
  else {
    //cprintf("else statement\n");
    mutex->queue[mutex->qsize] = curproc;
    mutex->qsize++;
    while(mutex->current != NULL){
      cprintf("mutex->current is busy\n"); 
    }
    while(mutex->current->tid != curproc->tid){
      //cprintf(".");
      cprintf("other process waiting..\n");
    }
  }
  //while(xchg(&mutex->lock, 1) != 0);
  cprintf("func: mutex_lock end\n");
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
 
  //xchg(&mutex->lock, 0);
  if(mutex->queue[mutex->qnext] != NULL){
    //mutex->queue[mutex->qnext] = mutex->current;
    mutex->current = mutex->queue[mutex->qnext];
    mutex->qnext++;
  }
  xchg(&mutex->lock, 0);
  //release(&mutex->lock);
  mutex->current = NULL;
  cprintf("func: mutex_unlock end\n");
  return 0;
}

int cond_init(struct cond_t *cond)
{

  cprintf("func: cond init\n");
  if(cond->valid == 1)
    return -2;

  cond->valid = 1;
  //initlock(&cond->lock, (char*)cond);
  cond->current = NULL;
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
  if(cond->valid != 1 || mutex->valid != 1){
    cprintf("return -2 error\n");
    return -2;
  }
  /*
  if(!holding(&mutex->lock)){
    cprintf("return -3 error\n");
    return -3;
  }*/
  
  if(mutex->current != curproc){
    cprintf("return -3-2 error\n");
    return -3;
  }
  mutex_unlock(mutex);

  if(cond->current == NULL)
    cond->current = curproc;

  else{
    cond->queue[cond->qsize] = curproc;
    cond->qsize++;
  }
  cprintf("going to sleep\n");
  //tsleep(curproc, &(cond->lock));
  //sleep(curproc, &(cond->lock));


  while(cond->active != 1 || cond->current != curproc);


  cprintf("wake up!!!!!!\n");
  mutex_lock(mutex);
  cprintf("successly end\n");
 
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
  if(cond->valid != 1){
    cprintf("cond_signal error!!\n");
    return -2;
  }
  
   
  if(cond->queue[cond->qnext] != NULL) {
    //twake(cond->queue[cond->qnext]);
    //wakeup(cond->queue[cond->qnext]);
    cond->current = cond->queue[cond->qnext];
    cond->qnext++;
    //wakeup(cond->current);
  }
  
  cprintf("func: cond_signal end\n"); 
  return 0;  
}
