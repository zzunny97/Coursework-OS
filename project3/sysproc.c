#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "synch.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_halt(void)
{
  outw(0x604, 0x2000);
  return 0;
}

int
sys_getnice(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return getnice(pid);
}

int
sys_setnice(void)
{
	int pid, value;
	
	if(argint(0, &pid) < 0)
		return -1;
	if(argint(1, &value) < 0)
		return -1;
	return setnice(pid, value);
}

void
sys_ps(void)
{
	int pid;

	if(argint(0, &pid) >= 0){
    ps(pid);
  }
}

int
sys_thread_create(void)
{
  int function, arg, stack;
  if(argint(0, &function) < 0)
    return -1;
  if(argint(1, &arg) < 0)
    return -1;
  if(argint(2, &stack) < 0)
    return -1;
  return thread_create((void*)function, (void*)arg, (void*)stack);
}

void
sys_thread_exit(void)
{
  int retval;
  if(argint(0, &retval) < 0)
    return;
  thread_exit((void*)retval);
}

int
sys_thread_join(void)
{
  int tid;
  int retval;
  if(argint(0, &tid) < 0)
    return -1;
  if(argint(1, &retval) < 0)
    return -1;
  return thread_join(tid, (void*)retval);
}

int
sys_gettid(void)
{
  int tid;
  if(argint(0, &tid) < 0)
    return -1;
  return gettid(tid);
}

int
sys_mutex_init(void)
{
  int mutex;
  if(argint(0, &mutex) < 0)
    return -1;
  return mutex_init((struct mutex_t*)mutex);
}
int
sys_mutex_lock(void)
{
  int mutex;
  if(argint(0, &mutex) < 0)
    return -1;
  return mutex_lock((struct mutex_t*)mutex);
}

int
sys_mutex_unlock(void)
{
  int mutex;
  if(argint(0, &mutex) < 0)
    return -1;
  return mutex_unlock((struct mutex_t*)mutex);
}


int
sys_cond_init(void)
{
  int cv;
  if(argint(0, &cv) < 0)
    return -1;
  return cond_init((struct cond_t*)cv);
}

int
sys_cond_wait(void)
{
  int mutex, cv;
  if(argint(0, &cv) < 0)
    return -1;
  if(argint(1, &mutex) < 0)
    return -1;
  return cond_wait((struct cond_t*)cv, (struct mutex_t*)mutex);
}

int
sys_cond_signal(void)
{
  int cv;
  if(argint(0, &cv) < 0)
    return -1;
  return cond_signal((struct cond_t*)cv);
}

