8450 // Console input and output.
8451 // Input is from the keyboard or serial port.
8452 // Output is written to the screen and serial port.
8453 
8454 #include "types.h"
8455 #include "defs.h"
8456 #include "param.h"
8457 #include "traps.h"
8458 #include "spinlock.h"
8459 #include "sleeplock.h"
8460 #include "fs.h"
8461 #include "file.h"
8462 #include "memlayout.h"
8463 #include "mmu.h"
8464 #include "proc.h"
8465 #include "x86.h"
8466 
8467 static void consputc(int);
8468 
8469 static int panicked = 0;
8470 
8471 static struct {
8472   struct spinlock lock;
8473   int locking;
8474 } cons;
8475 
8476 static void
8477 printint(int xx, int base, int sign)
8478 {
8479   static char digits[] = "0123456789abcdef";
8480   char buf[16];
8481   int i;
8482   uint x;
8483 
8484   if(sign && (sign = xx < 0))
8485     x = -xx;
8486   else
8487     x = xx;
8488 
8489   i = 0;
8490   do{
8491     buf[i++] = digits[x % base];
8492   }while((x /= base) != 0);
8493 
8494   if(sign)
8495     buf[i++] = '-';
8496 
8497   while(--i >= 0)
8498     consputc(buf[i]);
8499 }
8500 
8501 
8502 
8503 
8504 
8505 
8506 
8507 
8508 
8509 
8510 
8511 
8512 
8513 
8514 
8515 
8516 
8517 
8518 
8519 
8520 
8521 
8522 
8523 
8524 
8525 
8526 
8527 
8528 
8529 
8530 
8531 
8532 
8533 
8534 
8535 
8536 
8537 
8538 
8539 
8540 
8541 
8542 
8543 
8544 
8545 
8546 
8547 
8548 
8549 
8550 // Print to the console. only understands %d, %x, %p, %s.
8551 void
8552 cprintf(char *fmt, ...)
8553 {
8554   int i, c, locking;
8555   uint *argp;
8556   char *s;
8557 
8558   locking = cons.locking;
8559   if(locking)
8560     acquire(&cons.lock);
8561 
8562   if (fmt == 0)
8563     panic("null fmt");
8564 
8565   argp = (uint*)(void*)(&fmt + 1);
8566   for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
8567     if(c != '%'){
8568       consputc(c);
8569       continue;
8570     }
8571     c = fmt[++i] & 0xff;
8572     if(c == 0)
8573       break;
8574     switch(c){
8575     case 'd':
8576       printint(*argp++, 10, 1);
8577       break;
8578     case 'x':
8579     case 'p':
8580       printint(*argp++, 16, 0);
8581       break;
8582     case 's':
8583       if((s = (char*)*argp++) == 0)
8584         s = "(null)";
8585       for(; *s; s++)
8586         consputc(*s);
8587       break;
8588     case '%':
8589       consputc('%');
8590       break;
8591     default:
8592       // Print unknown % sequence to draw attention.
8593       consputc('%');
8594       consputc(c);
8595       break;
8596     }
8597   }
8598 
8599 
8600   if(locking)
8601     release(&cons.lock);
8602 }
8603 
8604 void
8605 panic(char *s)
8606 {
8607   int i;
8608   uint pcs[10];
8609 
8610   cli();
8611   cons.locking = 0;
8612   // use lapiccpunum so that we can call panic from mycpu()
8613   cprintf("lapicid %d: panic: ", lapicid());
8614   cprintf(s);
8615   cprintf("\n");
8616   getcallerpcs(&s, pcs);
8617   for(i=0; i<10; i++)
8618     cprintf(" %p", pcs[i]);
8619   panicked = 1; // freeze other CPU
8620   for(;;)
8621     ;
8622 }
8623 
8624 
8625 
8626 
8627 
8628 
8629 
8630 
8631 
8632 
8633 
8634 
8635 
8636 
8637 
8638 
8639 
8640 
8641 
8642 
8643 
8644 
8645 
8646 
8647 
8648 
8649 
8650 #define BACKSPACE 0x100
8651 #define CRTPORT 0x3d4
8652 static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory
8653 
8654 static void
8655 cgaputc(int c)
8656 {
8657   int pos;
8658 
8659   // Cursor position: col + 80*row.
8660   outb(CRTPORT, 14);
8661   pos = inb(CRTPORT+1) << 8;
8662   outb(CRTPORT, 15);
8663   pos |= inb(CRTPORT+1);
8664 
8665   if(c == '\n')
8666     pos += 80 - pos%80;
8667   else if(c == BACKSPACE){
8668     if(pos > 0) --pos;
8669   } else
8670     crt[pos++] = (c&0xff) | 0x0700;  // black on white
8671 
8672   if(pos < 0 || pos > 25*80)
8673     panic("pos under/overflow");
8674 
8675   if((pos/80) >= 24){  // Scroll up.
8676     memmove(crt, crt+80, sizeof(crt[0])*23*80);
8677     pos -= 80;
8678     memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
8679   }
8680 
8681   outb(CRTPORT, 14);
8682   outb(CRTPORT+1, pos>>8);
8683   outb(CRTPORT, 15);
8684   outb(CRTPORT+1, pos);
8685   crt[pos] = ' ' | 0x0700;
8686 }
8687 
8688 
8689 
8690 
8691 
8692 
8693 
8694 
8695 
8696 
8697 
8698 
8699 
8700 void
8701 consputc(int c)
8702 {
8703   if(panicked){
8704     cli();
8705     for(;;)
8706       ;
8707   }
8708 
8709   if(c == BACKSPACE){
8710     uartputc('\b'); uartputc(' '); uartputc('\b');
8711   } else
8712     uartputc(c);
8713   cgaputc(c);
8714 }
8715 
8716 #define INPUT_BUF 128
8717 struct {
8718   char buf[INPUT_BUF];
8719   uint r;  // Read index
8720   uint w;  // Write index
8721   uint e;  // Edit index
8722 } input;
8723 
8724 #define C(x)  ((x)-'@')  // Control-x
8725 
8726 void
8727 consoleintr(int (*getc)(void))
8728 {
8729   int c, doprocdump = 0;
8730 
8731   acquire(&cons.lock);
8732   while((c = getc()) >= 0){
8733     switch(c){
8734     case C('P'):  // Process listing.
8735       // procdump() locks cons.lock indirectly; invoke later
8736       doprocdump = 1;
8737       break;
8738     case C('U'):  // Kill line.
8739       while(input.e != input.w &&
8740             input.buf[(input.e-1) % INPUT_BUF] != '\n'){
8741         input.e--;
8742         consputc(BACKSPACE);
8743       }
8744       break;
8745     case C('H'): case '\x7f':  // Backspace
8746       if(input.e != input.w){
8747         input.e--;
8748         consputc(BACKSPACE);
8749       }
8750       break;
8751     default:
8752       if(c != 0 && input.e-input.r < INPUT_BUF){
8753         c = (c == '\r') ? '\n' : c;
8754         input.buf[input.e++ % INPUT_BUF] = c;
8755         consputc(c);
8756         if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
8757           input.w = input.e;
8758           wakeup(&input.r);
8759         }
8760       }
8761       break;
8762     }
8763   }
8764   release(&cons.lock);
8765   if(doprocdump) {
8766     procdump();  // now call procdump() wo. cons.lock held
8767   }
8768 }
8769 
8770 int
8771 consoleread(struct inode *ip, char *dst, int n)
8772 {
8773   uint target;
8774   int c;
8775 
8776   iunlock(ip);
8777   target = n;
8778   acquire(&cons.lock);
8779   while(n > 0){
8780     while(input.r == input.w){
8781       if(myproc()->killed){
8782         release(&cons.lock);
8783         ilock(ip);
8784         return -1;
8785       }
8786       sleep(&input.r, &cons.lock);
8787     }
8788     c = input.buf[input.r++ % INPUT_BUF];
8789     if(c == C('D')){  // EOF
8790       if(n < target){
8791         // Save ^D for next time, to make sure
8792         // caller gets a 0-byte result.
8793         input.r--;
8794       }
8795       break;
8796     }
8797     *dst++ = c;
8798     --n;
8799     if(c == '\n')
8800       break;
8801   }
8802   release(&cons.lock);
8803   ilock(ip);
8804 
8805   return target - n;
8806 }
8807 
8808 int
8809 consolewrite(struct inode *ip, char *buf, int n)
8810 {
8811   int i;
8812 
8813   iunlock(ip);
8814   acquire(&cons.lock);
8815   for(i = 0; i < n; i++)
8816     consputc(buf[i] & 0xff);
8817   release(&cons.lock);
8818   ilock(ip);
8819 
8820   return n;
8821 }
8822 
8823 void
8824 consoleinit(void)
8825 {
8826   initlock(&cons.lock, "console");
8827 
8828   devsw[CONSOLE].write = consolewrite;
8829   devsw[CONSOLE].read = consoleread;
8830   cons.locking = 1;
8831 
8832   ioapicenable(IRQ_KBD, 0);
8833 }
8834 
8835 
8836 
8837 
8838 
8839 
8840 
8841 
8842 
8843 
8844 
8845 
8846 
8847 
8848 
8849 
