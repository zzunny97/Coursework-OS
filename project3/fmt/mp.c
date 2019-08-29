7750 // Multiprocessor support
7751 // Search memory for MP description structures.
7752 // http://developer.intel.com/design/pentium/datashts/24201606.pdf
7753 
7754 #include "types.h"
7755 #include "defs.h"
7756 #include "param.h"
7757 #include "memlayout.h"
7758 #include "mp.h"
7759 #include "x86.h"
7760 #include "mmu.h"
7761 #include "proc.h"
7762 
7763 struct cpu cpus[NCPU];
7764 int ncpu;
7765 uchar ioapicid;
7766 
7767 static uchar
7768 sum(uchar *addr, int len)
7769 {
7770   int i, sum;
7771 
7772   sum = 0;
7773   for(i=0; i<len; i++)
7774     sum += addr[i];
7775   return sum;
7776 }
7777 
7778 // Look for an MP structure in the len bytes at addr.
7779 static struct mp*
7780 mpsearch1(uint a, int len)
7781 {
7782   uchar *e, *p, *addr;
7783 
7784   addr = P2V(a);
7785   e = addr+len;
7786   for(p = addr; p < e; p += sizeof(struct mp))
7787     if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
7788       return (struct mp*)p;
7789   return 0;
7790 }
7791 
7792 
7793 
7794 
7795 
7796 
7797 
7798 
7799 
7800 // Search for the MP Floating Pointer Structure, which according to the
7801 // spec is in one of the following three locations:
7802 // 1) in the first KB of the EBDA;
7803 // 2) in the last KB of system base memory;
7804 // 3) in the BIOS ROM between 0xE0000 and 0xFFFFF.
7805 static struct mp*
7806 mpsearch(void)
7807 {
7808   uchar *bda;
7809   uint p;
7810   struct mp *mp;
7811 
7812   bda = (uchar *) P2V(0x400);
7813   if((p = ((bda[0x0F]<<8)| bda[0x0E]) << 4)){
7814     if((mp = mpsearch1(p, 1024)))
7815       return mp;
7816   } else {
7817     p = ((bda[0x14]<<8)|bda[0x13])*1024;
7818     if((mp = mpsearch1(p-1024, 1024)))
7819       return mp;
7820   }
7821   return mpsearch1(0xF0000, 0x10000);
7822 }
7823 
7824 // Search for an MP configuration table.  For now,
7825 // don't accept the default configurations (physaddr == 0).
7826 // Check for correct signature, calculate the checksum and,
7827 // if correct, check the version.
7828 // To do: check extended table checksum.
7829 static struct mpconf*
7830 mpconfig(struct mp **pmp)
7831 {
7832   struct mpconf *conf;
7833   struct mp *mp;
7834 
7835   if((mp = mpsearch()) == 0 || mp->physaddr == 0)
7836     return 0;
7837   conf = (struct mpconf*) P2V((uint) mp->physaddr);
7838   if(memcmp(conf, "PCMP", 4) != 0)
7839     return 0;
7840   if(conf->version != 1 && conf->version != 4)
7841     return 0;
7842   if(sum((uchar*)conf, conf->length) != 0)
7843     return 0;
7844   *pmp = mp;
7845   return conf;
7846 }
7847 
7848 
7849 
7850 void
7851 mpinit(void)
7852 {
7853   uchar *p, *e;
7854   int ismp;
7855   struct mp *mp;
7856   struct mpconf *conf;
7857   struct mpproc *proc;
7858   struct mpioapic *ioapic;
7859 
7860   if((conf = mpconfig(&mp)) == 0)
7861     panic("Expect to run on an SMP");
7862   ismp = 1;
7863   lapic = (uint*)conf->lapicaddr;
7864   for(p=(uchar*)(conf+1), e=(uchar*)conf+conf->length; p<e; ){
7865     switch(*p){
7866     case MPPROC:
7867       proc = (struct mpproc*)p;
7868       if(ncpu < NCPU) {
7869         cpus[ncpu].apicid = proc->apicid;  // apicid may differ from ncpu
7870         ncpu++;
7871       }
7872       p += sizeof(struct mpproc);
7873       continue;
7874     case MPIOAPIC:
7875       ioapic = (struct mpioapic*)p;
7876       ioapicid = ioapic->apicno;
7877       p += sizeof(struct mpioapic);
7878       continue;
7879     case MPBUS:
7880     case MPIOINTR:
7881     case MPLINTR:
7882       p += 8;
7883       continue;
7884     default:
7885       ismp = 0;
7886       break;
7887     }
7888   }
7889   if(!ismp)
7890     panic("Didn't find a suitable machine");
7891 
7892   if(mp->imcrp){
7893     // Bochs doesn't support IMCR, so this doesn't run on Bochs.
7894     // But it would on real hardware.
7895     outb(0x22, 0x70);   // Select IMCR
7896     outb(0x23, inb(0x23) | 1);  // Mask external interrupts.
7897   }
7898 }
7899 
