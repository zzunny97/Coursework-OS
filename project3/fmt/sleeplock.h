4450 // Long-term locks for processes
4451 struct sleeplock {
4452   uint locked;       // Is the lock held?
4453   struct spinlock lk; // spinlock protecting this sleep lock
4454 
4455   // For debugging:
4456   char *name;        // Name of lock.
4457   int pid;           // Process holding lock
4458 };
4459 
4460 
4461 
4462 
4463 
4464 
4465 
4466 
4467 
4468 
4469 
4470 
4471 
4472 
4473 
4474 
4475 
4476 
4477 
4478 
4479 
4480 
4481 
4482 
4483 
4484 
4485 
4486 
4487 
4488 
4489 
4490 
4491 
4492 
4493 
4494 
4495 
4496 
4497 
4498 
4499 
