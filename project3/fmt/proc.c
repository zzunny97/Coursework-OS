2400 #include "types.h"
2401 #include "defs.h"
2402 #include "param.h"
2403 #include "memlayout.h"
2404 #include "mmu.h"
2405 #include "x86.h"
2406 #include "proc.h"
2407 #include "spinlock.h"
2408 #include "stat.h"
2409 #include "fcntl.h"
2410 
2411 struct {
2412   struct spinlock lock;
2413   struct proc proc[NPROC];
2414 } ptable;
2415 
2416 static struct proc *initproc;
2417 
2418 int nextpid = 1;
2419 extern void forkret(void);
2420 extern void trapret(void);
2421 
2422 static void wakeup1(void *chan);
2423 
2424   void
2425 pinit(void)
2426 {
2427   initlock(&ptable.lock, "ptable");
2428 }
2429 
2430 // Must be called with interrupts disabled
2431 int
2432 cpuid() {
2433   return mycpu()-cpus;
2434 }
2435 
2436 
2437 
2438 
2439 
2440 
2441 
2442 
2443 
2444 
2445 
2446 
2447 
2448 
2449 
2450 // Must be called with interrupts disabled to avoid the caller being
2451 // rescheduled between reading lapicid and running through the loop.
2452   struct cpu*
2453 mycpu(void)
2454 {
2455   int apicid, i;
2456 
2457   if(readeflags()&FL_IF)
2458     panic("mycpu called with interrupts enabled\n");
2459 
2460   apicid = lapicid();
2461   // APIC IDs are not guaranteed to be contiguous. Maybe we should have
2462   // a reverse map, or reserve a register to store &cpus[i].
2463   for (i = 0; i < ncpu; ++i) {
2464     if (cpus[i].apicid == apicid)
2465       return &cpus[i];
2466   }
2467   panic("unknown apicid\n");
2468 }
2469 
2470 // Disable interrupts so that we are not rescheduled
2471 // while reading proc from the cpu structure
2472 struct proc*
2473 myproc(void) {
2474   struct cpu *c;
2475   struct proc *p;
2476   pushcli();
2477   c = mycpu();
2478   p = c->proc;
2479   popcli();
2480   return p;
2481 }
2482 
2483 
2484 
2485 
2486 
2487 
2488 
2489 
2490 
2491 
2492 
2493 
2494 
2495 
2496 
2497 
2498 
2499 
2500 // Look in the process table for an UNUSED proc.
2501 // If found, change state to EMBRYO and initialize
2502 // state required to run in the kernel.
2503 // Otherwise return 0.
2504   static struct proc*
2505 allocproc(void)
2506 {
2507   struct proc *p;
2508   char *sp;
2509 
2510   acquire(&ptable.lock);
2511 
2512   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
2513     if(p->state == UNUSED)
2514       goto found;
2515 
2516   release(&ptable.lock);
2517   return 0;
2518 
2519 found:
2520   p->state = EMBRYO;
2521   p->pid = nextpid++;
2522 
2523 
2524   release(&ptable.lock);
2525 
2526   // Allocate kernel stack.
2527   if((p->kstack = kalloc()) == 0){
2528     p->state = UNUSED;
2529     return 0;
2530   }
2531   sp = p->kstack + KSTACKSIZE;
2532 
2533   // Leave room for trap frame.
2534   sp -= sizeof *p->tf;
2535   p->tf = (struct trapframe*)sp;
2536 
2537   // Set up new context to start executing at forkret,
2538   // which returns to trapret.
2539   sp -= 4;
2540   *(uint*)sp = (uint)trapret;
2541 
2542   sp -= sizeof *p->context;
2543   p->context = (struct context*)sp;
2544   memset(p->context, 0, sizeof *p->context);
2545   p->context->eip = (uint)forkret;
2546   p->tcnt = 0;
2547   return p;
2548 }
2549 
2550 
2551 // Set up first user process.
2552   void
2553 userinit(void)
2554 {
2555   struct proc *p;
2556   extern char _binary_initcode_start[], _binary_initcode_size[];
2557 
2558   p = allocproc();
2559 
2560   p->tid = 0;
2561   initproc = p;
2562   if((p->pgdir = setupkvm()) == 0)
2563     panic("userinit: out of memory?");
2564   inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
2565   p->sz = PGSIZE;
2566   memset(p->tf, 0, sizeof(*p->tf));
2567   p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
2568   p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
2569   p->tf->es = p->tf->ds;
2570   p->tf->ss = p->tf->ds;
2571   p->tf->eflags = FL_IF;
2572   p->tf->esp = PGSIZE;
2573   p->tf->eip = 0;  // beginning of initcode.S
2574 
2575   safestrcpy(p->name, "initcode", sizeof(p->name));
2576   p->cwd = namei("/");
2577 
2578   // this assignment to p->state lets other cores
2579   // run this process. the acquire forces the above
2580   // writes to be visible, and the lock is also needed
2581   // because the assignment might not be atomic.
2582   acquire(&ptable.lock);
2583 
2584   p->state = RUNNABLE;
2585   p->nice = 20;
2586   p->tid = 0;
2587   p->tcnt = 0;
2588   release(&ptable.lock);
2589 }
2590 
2591 
2592 
2593 
2594 
2595 
2596 
2597 
2598 
2599 
2600 // Grow current process's memory by n bytes.
2601 // Return 0 on success, -1 on failure.
2602   int
2603 growproc(int n)
2604 {
2605   uint sz;
2606   struct proc *curproc = myproc();
2607 
2608   sz = curproc->sz;
2609   if(n > 0){
2610     if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
2611       return -1;
2612   } else if(n < 0){
2613     if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
2614       return -1;
2615   }
2616   curproc->sz = sz;
2617   switchuvm(curproc);
2618   return 0;
2619 }
2620 
2621 // Create a new process copying p as the parent.
2622 // Sets up stack to return as if from system call.
2623 // Caller must set state of returned proc to RUNNABLE.
2624   int
2625 fork(void)
2626 {
2627   int i, pid;
2628   struct proc *np;
2629   struct proc *curproc = myproc();
2630   // Allocate process.
2631   if((np = allocproc()) == 0){
2632     return -1;
2633   }
2634 
2635   // Copy process state from proc.
2636   if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
2637     kfree(np->kstack);
2638     np->kstack = 0;
2639     np->state = UNUSED;
2640     return -1;
2641   }
2642   np->sz = curproc->sz;
2643   np->parent = curproc;
2644   *np->tf = *curproc->tf;
2645   np->nice = 20;
2646   // Clear %eax so that fork returns 0 in the child.
2647   np->tf->eax = 0;
2648   np->tid = 0;
2649   np->tcnt = 0;
2650   for(i = 0; i < NOFILE; i++)
2651     if(curproc->ofile[i])
2652       np->ofile[i] = filedup(curproc->ofile[i]);
2653   np->cwd = idup(curproc->cwd);
2654   safestrcpy(np->name, curproc->name, sizeof(curproc->name));
2655 
2656   pid = np->pid;
2657 
2658   acquire(&ptable.lock);
2659 
2660   np->state = RUNNABLE;
2661   release(&ptable.lock);
2662 
2663   return pid;
2664 }
2665 
2666 // Exit the current process.  Does not return.
2667 // An exited process remains in the zombie state
2668 // until its parent calls wait() to find out it exited.
2669   void
2670 exit(void)
2671 {
2672   struct proc *curproc = myproc();
2673   struct proc *p;
2674   int fd;
2675 
2676   if(curproc == initproc)
2677     panic("init exiting");
2678 
2679   // Close all open files.
2680   for(fd = 0; fd < NOFILE; fd++){
2681     if(curproc->ofile[fd]){
2682       fileclose(curproc->ofile[fd]);
2683       curproc->ofile[fd] = 0;
2684     }
2685   }
2686 
2687   begin_op();
2688   iput(curproc->cwd);
2689   end_op();
2690   curproc->cwd = 0;
2691 
2692   acquire(&ptable.lock);
2693 
2694   // Parent might be sleeping in wait().
2695   wakeup1(curproc->parent);
2696 
2697 
2698 
2699 
2700   // Pass abandoned children to init.
2701   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2702     if(p->parent == curproc){
2703       p->parent = initproc;
2704       if(p->state == ZOMBIE)
2705         wakeup1(initproc);
2706     }
2707   }
2708   /*
2709      for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2710      if(p->pid == curproc->pid){
2711      for(fd = 0; fd<NOFILE; fd++){
2712      if(p->ofile[fd]){
2713      fileclose(p->ofile[fd]);
2714      p->ofile[fd] = 0;
2715      }
2716      }
2717      begin_op();
2718      iput(p->cwd);
2719      end_op();
2720      p->pwd = 0;
2721      p->state = ZOMBIE;
2722      }
2723      }*/
2724   // Jump into the scheduler, never to return.
2725   curproc->state = ZOMBIE;
2726   sched();
2727   panic("zombie exit");
2728   if(curproc->tid == 1){
2729     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2730       if(p->pid == curproc->pid){
2731         for(fd = 0; fd<NOFILE; fd++){
2732           if(p->ofile[fd]){
2733             fileclose(p->ofile[fd]);
2734             p->ofile[fd] = 0;
2735           }
2736         }
2737         begin_op();
2738         iput(p->cwd);
2739         end_op();
2740         p->cwd = 0;
2741         p->state = ZOMBIE;
2742       }
2743     }
2744 
2745   }
2746 
2747 }
2748 
2749 
2750 // Wait for a child process to exit and return its pid.
2751 // Return -1 if this process has no children.
2752   int
2753 wait(void)
2754 {
2755   struct proc *p;
2756   int havekids, pid;
2757   struct proc *curproc = myproc();
2758 
2759   acquire(&ptable.lock);
2760   for(;;){
2761     // Scan through table looking for exited children.
2762     havekids = 0;
2763     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2764       if(p->parent != curproc)
2765         continue;
2766       havekids = 1;
2767       if(p->state == ZOMBIE){
2768         // Found one.
2769         pid = p->pid;
2770         kfree(p->kstack);
2771         p->kstack = 0;
2772         freevm(p->pgdir);
2773         p->pid = 0;
2774         p->parent = 0;
2775         p->name[0] = 0;
2776         p->killed = 0;
2777         p->state = UNUSED;
2778         release(&ptable.lock);
2779         return pid;
2780       }
2781     }
2782 
2783     // No point waiting if we don't have any children.
2784     if(!havekids || curproc->killed){
2785       release(&ptable.lock);
2786       return -1;
2787     }
2788 
2789     // Wait for children to exit.  (See wakeup1 call in proc_exit.)
2790     sleep(curproc, &ptable.lock);  //DOC: wait-sleep
2791   }
2792 }
2793 
2794 
2795 
2796 
2797 
2798 
2799 
2800 // Per-CPU process scheduler.
2801 // Each CPU calls scheduler() after setting itself up.
2802 // Scheduler never returns.  It loops, doing:
2803 //  - choose a process to run
2804 //  - swtch to start running that process
2805 //  - eventually that process transfers control
2806 //      via swtch back to the scheduler.
2807   void
2808 scheduler(void)
2809 {
2810   struct proc *p;
2811   struct cpu *c = mycpu();
2812   c->proc = 0;
2813 
2814   for(;;){
2815     // Enable interrupts on this processor.
2816     sti();
2817 
2818     // Loop over process table looking for process to run.
2819     acquire(&ptable.lock);
2820     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
2821       if(p->state != RUNNABLE)
2822         continue;
2823 
2824       // Switch to chosen process.  It is the process's job
2825       // to release ptable.lock and then reacquire it
2826       // before jumping back to us.
2827       c->proc = p;
2828       switchuvm(p);
2829       p->state = RUNNING;
2830 
2831       swtch(&(c->scheduler), p->context);
2832       switchkvm();
2833 
2834       // Process is done running for now.
2835       // It should have changed its p->state before coming back.
2836       c->proc = 0;
2837     }
2838     release(&ptable.lock);
2839 
2840   }
2841 }
2842 
2843 
2844 
2845 
2846 
2847 
2848 
2849 
2850 // Enter scheduler.  Must hold only ptable.lock
2851 // and have changed proc->state. Saves and restores
2852 // intena because intena is a property of this
2853 // kernel thread, not this CPU. It should
2854 // be proc->intena and proc->ncli, but that would
2855 // break in the few places where a lock is held but
2856 // there's no process.
2857   void
2858 sched(void)
2859 {
2860   int intena;
2861   struct proc *p = myproc();
2862 
2863   if(!holding(&ptable.lock))
2864     panic("sched ptable.lock");
2865   if(mycpu()->ncli != 1)
2866     panic("sched locks");
2867   if(p->state == RUNNING)
2868     panic("sched running");
2869   if(readeflags()&FL_IF)
2870     panic("sched interruptible");
2871   intena = mycpu()->intena;
2872   swtch(&p->context, mycpu()->scheduler);
2873   mycpu()->intena = intena;
2874 }
2875 
2876 // Give up the CPU for one scheduling round.
2877   void
2878 yield(void)
2879 {
2880   acquire(&ptable.lock);  //DOC: yieldlock
2881   myproc()->state = RUNNABLE;
2882   sched();
2883   release(&ptable.lock);
2884 }
2885 
2886 
2887 
2888 
2889 
2890 
2891 
2892 
2893 
2894 
2895 
2896 
2897 
2898 
2899 
2900 // A fork child's very first scheduling by scheduler()
2901 // will swtch here.  "Return" to user space.
2902   void
2903 forkret(void)
2904 {
2905   static int first = 1;
2906   // Still holding ptable.lock from scheduler.
2907   release(&ptable.lock);
2908 
2909   if (first) {
2910     // Some initialization functions must be run in the context
2911     // of a regular process (e.g., they call sleep), and thus cannot
2912     // be run from main().
2913     first = 0;
2914     iinit(ROOTDEV);
2915     initlog(ROOTDEV);
2916   }
2917 
2918   // Return to "caller", actually trapret (see allocproc).
2919 }
2920 
2921 // Atomically release lock and sleep on chan.
2922 // Reacquires lock when awakened.
2923   void
2924 sleep(void *chan, struct spinlock *lk)
2925 {
2926   struct proc *p = myproc();
2927 
2928   if(p == 0)
2929     panic("sleep");
2930 
2931   if(lk == 0)
2932     panic("sleep without lk");
2933 
2934   // Must acquire ptable.lock in order to
2935   // change p->state and then call sched.
2936   // Once we hold ptable.lock, we can be
2937   // guaranteed that we won't miss any wakeup
2938   // (wakeup runs with ptable.lock locked),
2939   // so it's okay to release lk.
2940   if(lk != &ptable.lock){  //DOC: sleeplock0
2941     acquire(&ptable.lock);  //DOC: sleeplock1
2942     release(lk);
2943   }
2944   // Go to sleep.
2945   p->chan = chan;
2946   p->state = SLEEPING;
2947 
2948   sched();
2949 
2950   // Tidy up.
2951   p->chan = 0;
2952 
2953   // Reacquire original lock.
2954   if(lk != &ptable.lock){  //DOC: sleeplock2
2955     release(&ptable.lock);
2956     acquire(lk);
2957   }
2958 }
2959 
2960 
2961 
2962 
2963 
2964 
2965 
2966 
2967 
2968 
2969 
2970 
2971 
2972 
2973 
2974 
2975 
2976 
2977 
2978 
2979 
2980 
2981 
2982 
2983 
2984 
2985 
2986 
2987 
2988 
2989 
2990 
2991 
2992 
2993 
2994 
2995 
2996 
2997 
2998 
2999 
3000 // Wake up all processes sleeping on chan.
3001 // The ptable lock must be held.
3002   static void
3003 wakeup1(void *chan)
3004 {
3005   struct proc *p;
3006 
3007   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
3008     if(p->state == SLEEPING && p->chan == chan)
3009       p->state = RUNNABLE;
3010 }
3011 
3012 // Wake up all processes sleeping on chan.
3013   void
3014 wakeup(void *chan)
3015 {
3016   acquire(&ptable.lock);
3017   wakeup1(chan);
3018   release(&ptable.lock);
3019 }
3020 
3021 // Kill the process with the given pid.
3022 // Process won't exit until it returns
3023 // to user space (see trap in trap.c).
3024   int
3025 kill(int pid)
3026 {
3027   struct proc *p;
3028 
3029   acquire(&ptable.lock);
3030   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3031     if(p->pid == pid){
3032       p->killed = 1;
3033       // Wake process from sleep if necessary.
3034       if(p->state == SLEEPING)
3035         p->state = RUNNABLE;
3036       release(&ptable.lock);
3037       return 0;
3038     }
3039   }
3040   release(&ptable.lock);
3041   return -1;
3042 }
3043 
3044 
3045 
3046 
3047 
3048 
3049 
3050 // Print a process listing to console.  For debugging.
3051 // Runs when user types ^P on console.
3052 // No lock to avoid wedging a stuck machine further.
3053   void
3054 procdump(void)
3055 {
3056   static char *states[] = {
3057     [UNUSED]    "unused",
3058     [EMBRYO]    "embryo",
3059     [SLEEPING]  "sleep ",
3060     [RUNNABLE]  "runble",
3061     [RUNNING]   "run   ",
3062     [ZOMBIE]    "zombie"
3063   };
3064   int i;
3065   struct proc *p;
3066   char *state;
3067   uint pc[10];
3068 
3069   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3070     if(p->state == UNUSED)
3071       continue;
3072     if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
3073       state = states[p->state];
3074     else
3075       state = "???";
3076     cprintf("%d %s %s", p->pid, state, p->name);
3077     if(p->state == SLEEPING){
3078       getcallerpcs((uint*)p->context->ebp+2, pc);
3079       for(i=0; i<10 && pc[i] != 0; i++)
3080         cprintf(" %p", pc[i]);
3081     }
3082     cprintf("\n");
3083   }
3084 }
3085 
3086 
3087 
3088 
3089 
3090 
3091 
3092 
3093 
3094 
3095 
3096 
3097 
3098 
3099 
3100   int
3101 getnice(int pid)
3102 {
3103   struct proc* p;
3104 
3105   acquire(&ptable.lock);
3106   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3107     if(p->pid == pid){
3108       release(&ptable.lock);
3109       cprintf("pid %d's nice value: %d\n", p->pid, p->nice);
3110       return p->nice;
3111     }
3112   }
3113   release(&ptable.lock);
3114   cprintf("pid %d not exists\n", p->pid);
3115   return -1;
3116 }
3117 
3118   int
3119 setnice(int pid, int value)
3120 {
3121   struct proc* p;
3122   if(!(0<= value && value<=39)){
3123     return -1;
3124   }
3125   acquire(&ptable.lock);
3126   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3127     if(p->pid == pid){
3128       p->nice = value;
3129       cprintf("pid %d's nice value has set as %d\n", p->pid, p->nice);
3130       release(&ptable.lock);
3131       return 0;
3132     }
3133   }
3134   release(&ptable.lock);
3135   cprintf("pid %d not exists\n", p->pid);
3136   return -1;
3137 }
3138 
3139 
3140 
3141 
3142 
3143 
3144 
3145 
3146 
3147 
3148 
3149 
3150 char* print_enum(enum procstate a)
3151 {
3152   if(a == RUNNING){
3153     return "RUNNING\t";
3154   }
3155   else if(a == SLEEPING){
3156     return "SLEEPING";
3157   }
3158   else if(a == EMBRYO){
3159     return "EMBRYO\t";
3160   }
3161   else if(a == UNUSED){
3162     return "UNUSED\t";
3163   }
3164   else if(a == RUNNABLE){
3165     return "RUNNABLE";
3166   }
3167   else{
3168     return "ZOMBIE\t";
3169   }
3170 }
3171 
3172   void
3173 ps(int pid)
3174 {
3175   struct proc* p;
3176 
3177   acquire(&ptable.lock);
3178   if(pid == 0) {
3179     for(p = ptable.proc; p<&ptable.proc[NPROC]; p++){
3180       if(p->pid != 0)
3181         cprintf("pid: %d\tnice: %d\tstatus: %s\tname: %s\n", p->pid, p->nice, print_enum(p->state), p->name);
3182     }
3183   }
3184   else{
3185     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3186       if(p->pid == pid){
3187         cprintf("pid: %d\tnice: %d\tstatus: %s\tname: %s\n", p->pid, p->nice, print_enum(p->state), p->name);
3188         break;
3189       }
3190     }
3191   }
3192   release(&ptable.lock);
3193 }
3194 
3195 
3196 
3197 
3198 
3199 
3200 int thread_create(void* (*function)(void*), void* arg, void* stack)
3201 {
3202   int i;// pid;
3203   struct proc *np;
3204   struct proc *curproc = myproc();
3205   //cprintf("curproc->tcnt: %d\n", curproc->tcnt);
3206   //cprintf("curproc's tid: %d\n", curproc->tid);
3207   if(curproc->tcnt == 8){
3208     cprintf("cannot alloate more threads(maxmium 8)\n");
3209     return -1;
3210   }
3211   // Allocate process.
3212   if((np = allocproc()) == 0){
3213     cprintf("allocproc error\n");
3214     return -1;
3215   }
3216 
3217   // Copy process state from proc.
3218   if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
3219     cprintf("copy process state error\n");
3220     kfree(np->kstack);
3221     np->kstack = 0;
3222     np->state = UNUSED;
3223     return -1;
3224   }
3225 
3226   np->pid = curproc->pid;
3227   np->sz = curproc->sz;
3228   np->parent = curproc;
3229   *np->tf = *curproc->tf;
3230   np->nice = 20;
3231   np->tcnt = curproc->tcnt;
3232   np->tcnt += 1;
3233   curproc->tcnt += 1;
3234   np->tid = np->tcnt;
3235   np->tf->eip = (uint)function;
3236 
3237   np->stack = stack;
3238   np->tf->ebp = (uint)stack + PGSIZE;
3239   np->tf->esp = (uint)stack + PGSIZE;
3240   np->tf->esp -= 4;
3241   //cprintf("np->tf->esp: %p\t np->tf->esp - 4: %p\n", (uint*)np->tf->esp, (uint*)(np->tf->esp -4));
3242   *((uint*)np->tf->esp) = (uint)arg;
3243 
3244   // Clear %eax so that fork returns 0 in the child.
3245   np->tf->eax = 0;
3246   for(i = 0; i < NOFILE; i++)
3247     if(curproc->ofile[i])
3248       np->ofile[i] = filedup(curproc->ofile[i]);
3249   np->cwd = idup(curproc->cwd);
3250   safestrcpy(np->name, curproc->name, sizeof(curproc->name));
3251 
3252   acquire(&ptable.lock);
3253   np->state = RUNNABLE;
3254   release(&ptable.lock);
3255   return np->tid;
3256 }
3257 
3258   void
3259 thread_exit(void* retval)
3260 {
3261   struct proc *curproc = myproc();
3262   struct proc *p;
3263   int fd;
3264 
3265   if(curproc == initproc)
3266     panic("init exiting");
3267 
3268   // Close all open files.
3269   for(fd = 0; fd < NOFILE; fd++){
3270     if(curproc->ofile[fd]){
3271       fileclose(curproc->ofile[fd]);
3272       curproc->ofile[fd] = 0;
3273     }
3274   }
3275 
3276   begin_op();
3277   iput(curproc->cwd);
3278   end_op();
3279   curproc->cwd = 0;
3280   //cprintf("retval: %d\n", (void*)retval);
3281   curproc->tf->eax = (uint)retval;
3282   acquire(&ptable.lock);
3283 
3284   // Parent might be sleeping in wait().
3285   wakeup1(curproc->parent);
3286   // Pass abandoned children to init.
3287   for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3288     if(p->parent == curproc){
3289       p->parent = initproc;
3290       if(p->state == ZOMBIE)
3291         wakeup1(initproc);
3292     }
3293   }
3294 
3295   // Jump into the scheduler, never to return.
3296   curproc->state = ZOMBIE;
3297   sched();
3298   panic("zombie exit");
3299 }
3300 // Wait for a child process to exit and return its pid.
3301 // Return -1 if this process has no children.
3302   int
3303 thread_join(int tid, void** retval)
3304 {
3305   //cprintf("func: thread_join\n");
3306   struct proc *p;
3307   int havekids;
3308   struct proc *curproc = myproc();
3309   //int pid;
3310   acquire(&ptable.lock);
3311   for(;;){
3312     // Scan through table looking for exited children.
3313     havekids = 0;
3314     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
3315       if(curproc->pid != p->pid || p->tid != tid)
3316         continue;
3317       //cprintf("have kid\n");
3318       /*
3319          if(p->tf->eax == (void*)0x87654321)
3320          cprintf("eax correct\n");
3321          else
3322          cprintf("eax wrong\n");*/
3323 
3324       havekids = 1;
3325       if(p->state == ZOMBIE){
3326         curproc->tcnt -= 1;
3327         *retval = (void*)p->tf->eax;
3328         //cprintf("current before thread cnt: %d\n", curproc->tcnt);
3329         //*(curproc->tcnt) -= 1;
3330         //cprintf("current after thread cnt: %d\n", curproc->tcnt);
3331         kfree(p->kstack);
3332         p->kstack = 0;
3333         //kfree(p->stack);
3334         freevm(p->pgdir);
3335         p->pid = 0;
3336         p->parent = 0;
3337         p->name[0] = 0;
3338         p->killed = 0;
3339         p->state = UNUSED;
3340         release(&ptable.lock);
3341         //cprintf("thread_join end\n");
3342         return 0;
3343       }
3344     }
3345 
3346 
3347 
3348 
3349 
3350     // No point waiting if we don't have any children.
3351     if(!havekids || curproc->killed){
3352       release(&ptable.lock);
3353       //cprintf("thread_join end with err (return -1)\n");
3354       return -1;
3355     }
3356 
3357     // Wait for children to exit.  (See wakeup1 call in proc_exit.)
3358     sleep(curproc, &ptable.lock);  //DOC: wait-sleep
3359   }
3360 }
3361 
3362 int gettid()
3363 {
3364   struct proc *curproc = myproc();
3365   return (int)curproc->tid;
3366 }
3367 
3368 
3369 
3370 
3371 
3372 
3373 
3374 
3375 
3376 
3377 
3378 
3379 
3380 
3381 
3382 
3383 
3384 
3385 
3386 
3387 
3388 
3389 
3390 
3391 
3392 
3393 
3394 
3395 
3396 
3397 
3398 
3399 
