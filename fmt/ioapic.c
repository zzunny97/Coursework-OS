8150 // The I/O APIC manages hardware interrupts for an SMP system.
8151 // http://www.intel.com/design/chipsets/datashts/29056601.pdf
8152 // See also picirq.c.
8153 
8154 #include "types.h"
8155 #include "defs.h"
8156 #include "traps.h"
8157 
8158 #define IOAPIC  0xFEC00000   // Default physical address of IO APIC
8159 
8160 #define REG_ID     0x00  // Register index: ID
8161 #define REG_VER    0x01  // Register index: version
8162 #define REG_TABLE  0x10  // Redirection table base
8163 
8164 // The redirection table starts at REG_TABLE and uses
8165 // two registers to configure each interrupt.
8166 // The first (low) register in a pair contains configuration bits.
8167 // The second (high) register contains a bitmask telling which
8168 // CPUs can serve that interrupt.
8169 #define INT_DISABLED   0x00010000  // Interrupt disabled
8170 #define INT_LEVEL      0x00008000  // Level-triggered (vs edge-)
8171 #define INT_ACTIVELOW  0x00002000  // Active low (vs high)
8172 #define INT_LOGICAL    0x00000800  // Destination is CPU id (vs APIC ID)
8173 
8174 volatile struct ioapic *ioapic;
8175 
8176 // IO APIC MMIO structure: write reg, then read or write data.
8177 struct ioapic {
8178   uint reg;
8179   uint pad[3];
8180   uint data;
8181 };
8182 
8183 static uint
8184 ioapicread(int reg)
8185 {
8186   ioapic->reg = reg;
8187   return ioapic->data;
8188 }
8189 
8190 static void
8191 ioapicwrite(int reg, uint data)
8192 {
8193   ioapic->reg = reg;
8194   ioapic->data = data;
8195 }
8196 
8197 
8198 
8199 
8200 void
8201 ioapicinit(void)
8202 {
8203   int i, id, maxintr;
8204 
8205   ioapic = (volatile struct ioapic*)IOAPIC;
8206   maxintr = (ioapicread(REG_VER) >> 16) & 0xFF;
8207   id = ioapicread(REG_ID) >> 24;
8208   if(id != ioapicid)
8209     cprintf("ioapicinit: id isn't equal to ioapicid; not a MP\n");
8210 
8211   // Mark all interrupts edge-triggered, active high, disabled,
8212   // and not routed to any CPUs.
8213   for(i = 0; i <= maxintr; i++){
8214     ioapicwrite(REG_TABLE+2*i, INT_DISABLED | (T_IRQ0 + i));
8215     ioapicwrite(REG_TABLE+2*i+1, 0);
8216   }
8217 }
8218 
8219 void
8220 ioapicenable(int irq, int cpunum)
8221 {
8222   // Mark interrupt edge-triggered, active high,
8223   // enabled, and routed to the given cpunum,
8224   // which happens to be that cpu's APIC ID.
8225   ioapicwrite(REG_TABLE+2*irq, T_IRQ0 + irq);
8226   ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24);
8227 }
8228 
8229 
8230 
8231 
8232 
8233 
8234 
8235 
8236 
8237 
8238 
8239 
8240 
8241 
8242 
8243 
8244 
8245 
8246 
8247 
8248 
8249 
