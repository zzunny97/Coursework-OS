7600 // See MultiProcessor Specification Version 1.[14]
7601 
7602 struct mp {             // floating pointer
7603   uchar signature[4];           // "_MP_"
7604   void *physaddr;               // phys addr of MP config table
7605   uchar length;                 // 1
7606   uchar specrev;                // [14]
7607   uchar checksum;               // all bytes must add up to 0
7608   uchar type;                   // MP system config type
7609   uchar imcrp;
7610   uchar reserved[3];
7611 };
7612 
7613 struct mpconf {         // configuration table header
7614   uchar signature[4];           // "PCMP"
7615   ushort length;                // total table length
7616   uchar version;                // [14]
7617   uchar checksum;               // all bytes must add up to 0
7618   uchar product[20];            // product id
7619   uint *oemtable;               // OEM table pointer
7620   ushort oemlength;             // OEM table length
7621   ushort entry;                 // entry count
7622   uint *lapicaddr;              // address of local APIC
7623   ushort xlength;               // extended table length
7624   uchar xchecksum;              // extended table checksum
7625   uchar reserved;
7626 };
7627 
7628 struct mpproc {         // processor table entry
7629   uchar type;                   // entry type (0)
7630   uchar apicid;                 // local APIC id
7631   uchar version;                // local APIC verison
7632   uchar flags;                  // CPU flags
7633     #define MPBOOT 0x02           // This proc is the bootstrap processor.
7634   uchar signature[4];           // CPU signature
7635   uint feature;                 // feature flags from CPUID instruction
7636   uchar reserved[8];
7637 };
7638 
7639 struct mpioapic {       // I/O APIC table entry
7640   uchar type;                   // entry type (2)
7641   uchar apicno;                 // I/O APIC id
7642   uchar version;                // I/O APIC version
7643   uchar flags;                  // I/O APIC flags
7644   uint *addr;                  // I/O APIC address
7645 };
7646 
7647 
7648 
7649 
7650 // Table entry types
7651 #define MPPROC    0x00  // One per processor
7652 #define MPBUS     0x01  // One per bus
7653 #define MPIOAPIC  0x02  // One per I/O APIC
7654 #define MPIOINTR  0x03  // One per bus interrupt source
7655 #define MPLINTR   0x04  // One per system interrupt source
7656 
7657 
7658 
7659 
7660 
7661 
7662 
7663 
7664 
7665 
7666 
7667 
7668 
7669 
7670 
7671 
7672 
7673 
7674 
7675 
7676 
7677 
7678 
7679 
7680 
7681 
7682 
7683 
7684 
7685 
7686 
7687 
7688 
7689 
7690 
7691 
7692 
7693 
7694 
7695 
7696 
7697 
7698 
7699 
7700 // Blank page.
7701 
7702 
7703 
7704 
7705 
7706 
7707 
7708 
7709 
7710 
7711 
7712 
7713 
7714 
7715 
7716 
7717 
7718 
7719 
7720 
7721 
7722 
7723 
7724 
7725 
7726 
7727 
7728 
7729 
7730 
7731 
7732 
7733 
7734 
7735 
7736 
7737 
7738 
7739 
7740 
7741 
7742 
7743 
7744 
7745 
7746 
7747 
7748 
7749 
