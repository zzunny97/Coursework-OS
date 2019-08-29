7300 #include "types.h"
7301 #include "defs.h"
7302 #include "param.h"
7303 #include "mmu.h"
7304 #include "proc.h"
7305 #include "fs.h"
7306 #include "spinlock.h"
7307 #include "sleeplock.h"
7308 #include "file.h"
7309 
7310 #define PIPESIZE 512
7311 
7312 struct pipe {
7313   struct spinlock lock;
7314   char data[PIPESIZE];
7315   uint nread;     // number of bytes read
7316   uint nwrite;    // number of bytes written
7317   int readopen;   // read fd is still open
7318   int writeopen;  // write fd is still open
7319 };
7320 
7321 int
7322 pipealloc(struct file **f0, struct file **f1)
7323 {
7324   struct pipe *p;
7325 
7326   p = 0;
7327   *f0 = *f1 = 0;
7328   if((*f0 = filealloc()) == 0 || (*f1 = filealloc()) == 0)
7329     goto bad;
7330   if((p = (struct pipe*)kalloc()) == 0)
7331     goto bad;
7332   p->readopen = 1;
7333   p->writeopen = 1;
7334   p->nwrite = 0;
7335   p->nread = 0;
7336   initlock(&p->lock, "pipe");
7337   (*f0)->type = FD_PIPE;
7338   (*f0)->readable = 1;
7339   (*f0)->writable = 0;
7340   (*f0)->pipe = p;
7341   (*f1)->type = FD_PIPE;
7342   (*f1)->readable = 0;
7343   (*f1)->writable = 1;
7344   (*f1)->pipe = p;
7345   return 0;
7346 
7347 
7348 
7349 
7350  bad:
7351   if(p)
7352     kfree((char*)p);
7353   if(*f0)
7354     fileclose(*f0);
7355   if(*f1)
7356     fileclose(*f1);
7357   return -1;
7358 }
7359 
7360 void
7361 pipeclose(struct pipe *p, int writable)
7362 {
7363   acquire(&p->lock);
7364   if(writable){
7365     p->writeopen = 0;
7366     wakeup(&p->nread);
7367   } else {
7368     p->readopen = 0;
7369     wakeup(&p->nwrite);
7370   }
7371   if(p->readopen == 0 && p->writeopen == 0){
7372     release(&p->lock);
7373     kfree((char*)p);
7374   } else
7375     release(&p->lock);
7376 }
7377 
7378 
7379 int
7380 pipewrite(struct pipe *p, char *addr, int n)
7381 {
7382   int i;
7383 
7384   acquire(&p->lock);
7385   for(i = 0; i < n; i++){
7386     while(p->nwrite == p->nread + PIPESIZE){  //DOC: pipewrite-full
7387       if(p->readopen == 0 || myproc()->killed){
7388         release(&p->lock);
7389         return -1;
7390       }
7391       wakeup(&p->nread);
7392       sleep(&p->nwrite, &p->lock);  //DOC: pipewrite-sleep
7393     }
7394     p->data[p->nwrite++ % PIPESIZE] = addr[i];
7395   }
7396   wakeup(&p->nread);  //DOC: pipewrite-wakeup1
7397   release(&p->lock);
7398   return n;
7399 }
7400 int
7401 piperead(struct pipe *p, char *addr, int n)
7402 {
7403   int i;
7404 
7405   acquire(&p->lock);
7406   while(p->nread == p->nwrite && p->writeopen){  //DOC: pipe-empty
7407     if(myproc()->killed){
7408       release(&p->lock);
7409       return -1;
7410     }
7411     sleep(&p->nread, &p->lock); //DOC: piperead-sleep
7412   }
7413   for(i = 0; i < n; i++){  //DOC: piperead-copy
7414     if(p->nread == p->nwrite)
7415       break;
7416     addr[i] = p->data[p->nread++ % PIPESIZE];
7417   }
7418   wakeup(&p->nwrite);  //DOC: piperead-wakeup
7419   release(&p->lock);
7420   return i;
7421 }
7422 
7423 
7424 
7425 
7426 
7427 
7428 
7429 
7430 
7431 
7432 
7433 
7434 
7435 
7436 
7437 
7438 
7439 
7440 
7441 
7442 
7443 
7444 
7445 
7446 
7447 
7448 
7449 
