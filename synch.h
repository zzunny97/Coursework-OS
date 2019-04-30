#define NTHREAD 8


struct mutex_t{
  int valid;
  struct spinlock lock;
  struct proc *current;
  //struct proc *queue[NTHREAD-1];
  struct proc *queue[10];
  int qsize;
  int qnext;
};

struct cond_t{
  int valid;
  int active;
  struct spinlock lock;
  struct proc *current;
  //struct proc *queue[NTHREAD-1];
  struct proc *queue[10];
  int qsize;
  int qnext;
};

int mutex_init(struct mutex_t* mutex);
int mutex_lock(struct mutex_t* mutex);
int mutex_unlock(struct mutex_t* mutex);

int cond_init(struct cond_t *cond);
int cond_wait(struct cond_t *cond, struct mutex_t *mutex);
int cond_signal(struct cond_t *cond);
