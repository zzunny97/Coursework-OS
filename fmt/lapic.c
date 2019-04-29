7900 // The local APIC manages internal (non-I/O) interrupts.
7901 // See Chapter 8 & Appendix C of Intel processor manual volume 3.
7902 
7903 #include "param.h"
7904 #include "types.h"
7905 #include "defs.h"
7906 #include "date.h"
7907 #include "memlayout.h"
7908 #include "traps.h"
7909 #include "mmu.h"
7910 #include "x86.h"
7911 
7912 // Local APIC registers, divided by 4 for use as uint[] indices.
7913 #define ID      (0x0020/4)   // ID
7914 #define VER     (0x0030/4)   // Version
7915 #define TPR     (0x0080/4)   // Task Priority
7916 #define EOI     (0x00B0/4)   // EOI
7917 #define SVR     (0x00F0/4)   // Spurious Interrupt Vector
7918   #define ENABLE     0x00000100   // Unit Enable
7919 #define ESR     (0x0280/4)   // Error Status
7920 #define ICRLO   (0x0300/4)   // Interrupt Command
7921   #define INIT       0x00000500   // INIT/RESET
7922   #define STARTUP    0x00000600   // Startup IPI
7923   #define DELIVS     0x00001000   // Delivery status
7924   #define ASSERT     0x00004000   // Assert interrupt (vs deassert)
7925   #define DEASSERT   0x00000000
7926   #define LEVEL      0x00008000   // Level triggered
7927   #define BCAST      0x00080000   // Send to all APICs, including self.
7928   #define BUSY       0x00001000
7929   #define FIXED      0x00000000
7930 #define ICRHI   (0x0310/4)   // Interrupt Command [63:32]
7931 #define TIMER   (0x0320/4)   // Local Vector Table 0 (TIMER)
7932   #define X1         0x0000000B   // divide counts by 1
7933   #define PERIODIC   0x00020000   // Periodic
7934 #define PCINT   (0x0340/4)   // Performance Counter LVT
7935 #define LINT0   (0x0350/4)   // Local Vector Table 1 (LINT0)
7936 #define LINT1   (0x0360/4)   // Local Vector Table 2 (LINT1)
7937 #define ERROR   (0x0370/4)   // Local Vector Table 3 (ERROR)
7938   #define MASKED     0x00010000   // Interrupt masked
7939 #define TICR    (0x0380/4)   // Timer Initial Count
7940 #define TCCR    (0x0390/4)   // Timer Current Count
7941 #define TDCR    (0x03E0/4)   // Timer Divide Configuration
7942 
7943 volatile uint *lapic;  // Initialized in mp.c
7944 
7945 
7946 
7947 
7948 
7949 
7950 static void
7951 lapicw(int index, int value)
7952 {
7953   lapic[index] = value;
7954   lapic[ID];  // wait for write to finish, by reading
7955 }
7956 
7957 void
7958 lapicinit(void)
7959 {
7960   if(!lapic)
7961     return;
7962 
7963   // Enable local APIC; set spurious interrupt vector.
7964   lapicw(SVR, ENABLE | (T_IRQ0 + IRQ_SPURIOUS));
7965 
7966   // The timer repeatedly counts down at bus frequency
7967   // from lapic[TICR] and then issues an interrupt.
7968   // If xv6 cared more about precise timekeeping,
7969   // TICR would be calibrated using an external time source.
7970   lapicw(TDCR, X1);
7971   lapicw(TIMER, PERIODIC | (T_IRQ0 + IRQ_TIMER));
7972   lapicw(TICR, 10000000);
7973 
7974   // Disable logical interrupt lines.
7975   lapicw(LINT0, MASKED);
7976   lapicw(LINT1, MASKED);
7977 
7978   // Disable performance counter overflow interrupts
7979   // on machines that provide that interrupt entry.
7980   if(((lapic[VER]>>16) & 0xFF) >= 4)
7981     lapicw(PCINT, MASKED);
7982 
7983   // Map error interrupt to IRQ_ERROR.
7984   lapicw(ERROR, T_IRQ0 + IRQ_ERROR);
7985 
7986   // Clear error status register (requires back-to-back writes).
7987   lapicw(ESR, 0);
7988   lapicw(ESR, 0);
7989 
7990   // Ack any outstanding interrupts.
7991   lapicw(EOI, 0);
7992 
7993   // Send an Init Level De-Assert to synchronise arbitration ID's.
7994   lapicw(ICRHI, 0);
7995   lapicw(ICRLO, BCAST | INIT | LEVEL);
7996   while(lapic[ICRLO] & DELIVS)
7997     ;
7998 
7999 
8000   // Enable interrupts on the APIC (but not on the processor).
8001   lapicw(TPR, 0);
8002 }
8003 
8004 int
8005 lapicid(void)
8006 {
8007   if (!lapic)
8008     return 0;
8009   return lapic[ID] >> 24;
8010 }
8011 
8012 // Acknowledge interrupt.
8013 void
8014 lapiceoi(void)
8015 {
8016   if(lapic)
8017     lapicw(EOI, 0);
8018 }
8019 
8020 // Spin for a given number of microseconds.
8021 // On real hardware would want to tune this dynamically.
8022 void
8023 microdelay(int us)
8024 {
8025 }
8026 
8027 #define CMOS_PORT    0x70
8028 #define CMOS_RETURN  0x71
8029 
8030 // Start additional processor running entry code at addr.
8031 // See Appendix B of MultiProcessor Specification.
8032 void
8033 lapicstartap(uchar apicid, uint addr)
8034 {
8035   int i;
8036   ushort *wrv;
8037 
8038   // "The BSP must initialize CMOS shutdown code to 0AH
8039   // and the warm reset vector (DWORD based at 40:67) to point at
8040   // the AP startup code prior to the [universal startup algorithm]."
8041   outb(CMOS_PORT, 0xF);  // offset 0xF is shutdown code
8042   outb(CMOS_PORT+1, 0x0A);
8043   wrv = (ushort*)P2V((0x40<<4 | 0x67));  // Warm reset vector
8044   wrv[0] = 0;
8045   wrv[1] = addr >> 4;
8046 
8047 
8048 
8049 
8050   // "Universal startup algorithm."
8051   // Send INIT (level-triggered) interrupt to reset other CPU.
8052   lapicw(ICRHI, apicid<<24);
8053   lapicw(ICRLO, INIT | LEVEL | ASSERT);
8054   microdelay(200);
8055   lapicw(ICRLO, INIT | LEVEL);
8056   microdelay(100);    // should be 10ms, but too slow in Bochs!
8057 
8058   // Send startup IPI (twice!) to enter code.
8059   // Regular hardware is supposed to only accept a STARTUP
8060   // when it is in the halted state due to an INIT.  So the second
8061   // should be ignored, but it is part of the official Intel algorithm.
8062   // Bochs complains about the second one.  Too bad for Bochs.
8063   for(i = 0; i < 2; i++){
8064     lapicw(ICRHI, apicid<<24);
8065     lapicw(ICRLO, STARTUP | (addr>>12));
8066     microdelay(200);
8067   }
8068 }
8069 
8070 #define CMOS_STATA   0x0a
8071 #define CMOS_STATB   0x0b
8072 #define CMOS_UIP    (1 << 7)        // RTC update in progress
8073 
8074 #define SECS    0x00
8075 #define MINS    0x02
8076 #define HOURS   0x04
8077 #define DAY     0x07
8078 #define MONTH   0x08
8079 #define YEAR    0x09
8080 
8081 static uint
8082 cmos_read(uint reg)
8083 {
8084   outb(CMOS_PORT,  reg);
8085   microdelay(200);
8086 
8087   return inb(CMOS_RETURN);
8088 }
8089 
8090 static void
8091 fill_rtcdate(struct rtcdate *r)
8092 {
8093   r->second = cmos_read(SECS);
8094   r->minute = cmos_read(MINS);
8095   r->hour   = cmos_read(HOURS);
8096   r->day    = cmos_read(DAY);
8097   r->month  = cmos_read(MONTH);
8098   r->year   = cmos_read(YEAR);
8099 }
8100 // qemu seems to use 24-hour GWT and the values are BCD encoded
8101 void
8102 cmostime(struct rtcdate *r)
8103 {
8104   struct rtcdate t1, t2;
8105   int sb, bcd;
8106 
8107   sb = cmos_read(CMOS_STATB);
8108 
8109   bcd = (sb & (1 << 2)) == 0;
8110 
8111   // make sure CMOS doesn't modify time while we read it
8112   for(;;) {
8113     fill_rtcdate(&t1);
8114     if(cmos_read(CMOS_STATA) & CMOS_UIP)
8115         continue;
8116     fill_rtcdate(&t2);
8117     if(memcmp(&t1, &t2, sizeof(t1)) == 0)
8118       break;
8119   }
8120 
8121   // convert
8122   if(bcd) {
8123 #define    CONV(x)     (t1.x = ((t1.x >> 4) * 10) + (t1.x & 0xf))
8124     CONV(second);
8125     CONV(minute);
8126     CONV(hour  );
8127     CONV(day   );
8128     CONV(month );
8129     CONV(year  );
8130 #undef     CONV
8131   }
8132 
8133   *r = t1;
8134   r->year += 2000;
8135 }
8136 
8137 
8138 
8139 
8140 
8141 
8142 
8143 
8144 
8145 
8146 
8147 
8148 
8149 
