2300 // Per-CPU state
2301 struct cpu {
2302   uchar apicid;                // Local APIC ID
2303   struct context *scheduler;   // swtch() here to enter scheduler
2304   struct taskstate ts;         // Used by x86 to find stack for interrupt
2305   struct segdesc gdt[NSEGS];   // x86 global descriptor table
2306   volatile uint started;       // Has the CPU started?
2307   int ncli;                    // Depth of pushcli nesting.
2308   int intena;                  // Were interrupts enabled before pushcli?
2309   struct proc *proc;           // The process running on this cpu or null
2310 };
2311 
2312 extern struct cpu cpus[NCPU];
2313 extern int ncpu;
2314 
2315 
2316 // Saved registers for kernel context switches.
2317 // Don't need to save all the segment registers (%cs, etc),
2318 // because they are constant across kernel contexts.
2319 // Don't need to save %eax, %ecx, %edx, because the
2320 // x86 convention is that the caller has saved them.
2321 // Contexts are stored at the bottom of the stack they
2322 // describe; the stack pointer is the address of the context.
2323 // The layout of the context matches the layout of the stack in swtch.S
2324 // at the "Switch stacks" comment. Switch doesn't save eip explicitly,
2325 // but it is on the stack and allocproc() manipulates it.
2326 struct context {
2327   uint edi;
2328   uint esi;
2329   uint ebx;
2330   uint ebp;
2331   uint eip;
2332 };
2333 
2334 enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
2335 
2336 // Per-process state
2337 struct proc {
2338   uint sz;                     // Size of process memory (bytes)
2339   pde_t* pgdir;                // Page table
2340   char *kstack;                // Bottom of kernel stack for this process
2341   enum procstate state;        // Process state
2342   int pid;                     // Process ID
2343   int nice;                    // Nice Value of Process
2344   struct proc *parent;         // Parent process
2345   struct trapframe *tf;        // Trap frame for current syscall
2346   struct context *context;     // swtch() here to run process
2347   void *chan;                  // If non-zero, sleeping on chan
2348   int killed;                  // If non-zero, have been killed
2349   struct file *ofile[NOFILE];  // Open files
2350   struct inode *cwd;           // Current directory
2351   char name[16];               // Process name (debugging)
2352   void* stack;
2353   int tid;
2354   int tcnt;
2355 };
2356 
2357 // Process memory is laid out contiguously, low addresses first:
2358 //   text
2359 //   original data and bss
2360 //   fixed-size stack
2361 //   expandable heap
2362 
2363 
2364 
2365 
2366 
2367 
2368 
2369 
2370 
2371 
2372 
2373 
2374 
2375 
2376 
2377 
2378 
2379 
2380 
2381 
2382 
2383 
2384 
2385 
2386 
2387 
2388 
2389 
2390 
2391 
2392 
2393 
2394 
2395 
2396 
2397 
2398 
2399 
