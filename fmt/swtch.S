3400 # Context switch
3401 #
3402 #   void swtch(struct context **old, struct context *new);
3403 #
3404 # Save the current registers on the stack, creating
3405 # a struct context, and save its address in *old.
3406 # Switch stacks to new and pop previously-saved registers.
3407 
3408 .globl swtch
3409 swtch:
3410   movl 4(%esp), %eax
3411   movl 8(%esp), %edx
3412 
3413   # Save old callee-saved registers
3414   pushl %ebp
3415   pushl %ebx
3416   pushl %esi
3417   pushl %edi
3418 
3419   # Switch stacks
3420   movl %esp, (%eax)
3421   movl %edx, %esp
3422 
3423   # Load new callee-saved registers
3424   popl %edi
3425   popl %esi
3426   popl %ebx
3427   popl %ebp
3428   ret
3429 
3430 
3431 
3432 
3433 
3434 
3435 
3436 
3437 
3438 
3439 
3440 
3441 
3442 
3443 
3444 
3445 
3446 
3447 
3448 
3449 
