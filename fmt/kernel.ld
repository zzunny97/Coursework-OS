9850 /* Simple linker script for the JOS kernel.
9851    See the GNU ld 'info' manual ("info ld") to learn the syntax. */
9852 
9853 OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
9854 OUTPUT_ARCH(i386)
9855 ENTRY(_start)
9856 
9857 SECTIONS
9858 {
9859 	/* Link the kernel at this address: "." means the current address */
9860         /* Must be equal to KERNLINK */
9861 	. = 0x80100000;
9862 
9863 	.text : AT(0x100000) {
9864 		*(.text .stub .text.* .gnu.linkonce.t.*)
9865 	}
9866 
9867 	PROVIDE(etext = .);	/* Define the 'etext' symbol to this value */
9868 
9869 	.rodata : {
9870 		*(.rodata .rodata.* .gnu.linkonce.r.*)
9871 	}
9872 
9873 	/* Include debugging information in kernel memory */
9874 	.stab : {
9875 		PROVIDE(__STAB_BEGIN__ = .);
9876 		*(.stab);
9877 		PROVIDE(__STAB_END__ = .);
9878 		BYTE(0)		/* Force the linker to allocate space
9879 				   for this section */
9880 	}
9881 
9882 	.stabstr : {
9883 		PROVIDE(__STABSTR_BEGIN__ = .);
9884 		*(.stabstr);
9885 		PROVIDE(__STABSTR_END__ = .);
9886 		BYTE(0)		/* Force the linker to allocate space
9887 				   for this section */
9888 	}
9889 
9890 	/* Adjust the address for the data segment to the next page */
9891 	. = ALIGN(0x1000);
9892 
9893 	/* Conventionally, Unix linkers provide pseudo-symbols
9894 	 * etext, edata, and end, at the end of the text, data, and bss.
9895 	 * For the kernel mapping, we need the address at the beginning
9896 	 * of the data section, but that's not one of the conventional
9897 	 * symbols, because the convention started before there was a
9898 	 * read-only rodata section between text and data. */
9899 	PROVIDE(data = .);
9900 	/* The data segment */
9901 	.data : {
9902 		*(.data)
9903 	}
9904 
9905 	PROVIDE(edata = .);
9906 
9907 	.bss : {
9908 		*(.bss)
9909 	}
9910 
9911 	PROVIDE(end = .);
9912 
9913 	/DISCARD/ : {
9914 		*(.eh_frame .note.GNU-stack)
9915 	}
9916 }
9917 
9918 
9919 
9920 
9921 
9922 
9923 
9924 
9925 
9926 
9927 
9928 
9929 
9930 
9931 
9932 
9933 
9934 
9935 
9936 
9937 
9938 
9939 
9940 
9941 
9942 
9943 
9944 
9945 
9946 
9947 
9948 
9949 
