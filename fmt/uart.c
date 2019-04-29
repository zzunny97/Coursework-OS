8850 // Intel 8250 serial port (UART).
8851 
8852 #include "types.h"
8853 #include "defs.h"
8854 #include "param.h"
8855 #include "traps.h"
8856 #include "spinlock.h"
8857 #include "sleeplock.h"
8858 #include "fs.h"
8859 #include "file.h"
8860 #include "mmu.h"
8861 #include "proc.h"
8862 #include "x86.h"
8863 
8864 #define COM1    0x3f8
8865 
8866 static int uart;    // is there a uart?
8867 
8868 void
8869 uartinit(void)
8870 {
8871   char *p;
8872 
8873   // Turn off the FIFO
8874   outb(COM1+2, 0);
8875 
8876   // 9600 baud, 8 data bits, 1 stop bit, parity off.
8877   outb(COM1+3, 0x80);    // Unlock divisor
8878   outb(COM1+0, 115200/9600);
8879   outb(COM1+1, 0);
8880   outb(COM1+3, 0x03);    // Lock divisor, 8 data bits.
8881   outb(COM1+4, 0);
8882   outb(COM1+1, 0x01);    // Enable receive interrupts.
8883 
8884   // If status is 0xFF, no serial port.
8885   if(inb(COM1+5) == 0xFF)
8886     return;
8887   uart = 1;
8888 
8889   // Acknowledge pre-existing interrupt conditions;
8890   // enable interrupts.
8891   inb(COM1+2);
8892   inb(COM1+0);
8893   ioapicenable(IRQ_COM1, 0);
8894 
8895   // Announce that we're here.
8896   for(p="xv6...\n"; *p; p++)
8897     uartputc(*p);
8898 }
8899 
8900 void
8901 uartputc(int c)
8902 {
8903   int i;
8904 
8905   if(!uart)
8906     return;
8907   for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++)
8908     microdelay(10);
8909   outb(COM1+0, c);
8910 }
8911 
8912 static int
8913 uartgetc(void)
8914 {
8915   if(!uart)
8916     return -1;
8917   if(!(inb(COM1+5) & 0x01))
8918     return -1;
8919   return inb(COM1+0);
8920 }
8921 
8922 void
8923 uartintr(void)
8924 {
8925   consoleintr(uartgetc);
8926 }
8927 
8928 
8929 
8930 
8931 
8932 
8933 
8934 
8935 
8936 
8937 
8938 
8939 
8940 
8941 
8942 
8943 
8944 
8945 
8946 
8947 
8948 
8949 
