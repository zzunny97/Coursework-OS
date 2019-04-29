4600 // On-disk file system format.
4601 // Both the kernel and user programs use this header file.
4602 
4603 
4604 #define ROOTINO 1  // root i-number
4605 #define BSIZE 512  // block size
4606 
4607 // Disk layout:
4608 // [ boot block | super block | log | inode blocks |
4609 //                                          free bit map | data blocks]
4610 //
4611 // mkfs computes the super block and builds an initial file system. The
4612 // super block describes the disk layout:
4613 struct superblock {
4614   uint size;         // Size of file system image (blocks)
4615   uint nblocks;      // Number of data blocks
4616   uint ninodes;      // Number of inodes.
4617   uint nlog;         // Number of log blocks
4618   uint logstart;     // Block number of first log block
4619   uint inodestart;   // Block number of first inode block
4620   uint bmapstart;    // Block number of first free map block
4621 };
4622 
4623 #define NDIRECT 12
4624 #define NINDIRECT (BSIZE / sizeof(uint))
4625 #define MAXFILE (NDIRECT + NINDIRECT)
4626 
4627 // On-disk inode structure
4628 struct dinode {
4629   short type;           // File type
4630   short major;          // Major device number (T_DEV only)
4631   short minor;          // Minor device number (T_DEV only)
4632   short nlink;          // Number of links to inode in file system
4633   uint size;            // Size of file (bytes)
4634   uint addrs[NDIRECT+1];   // Data block addresses
4635 };
4636 
4637 
4638 
4639 
4640 
4641 
4642 
4643 
4644 
4645 
4646 
4647 
4648 
4649 
4650 // Inodes per block.
4651 #define IPB           (BSIZE / sizeof(struct dinode))
4652 
4653 // Block containing inode i
4654 #define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)
4655 
4656 // Bitmap bits per block
4657 #define BPB           (BSIZE*8)
4658 
4659 // Block of free map containing bit for block b
4660 #define BBLOCK(b, sb) (b/BPB + sb.bmapstart)
4661 
4662 // Directory is a file containing a sequence of dirent structures.
4663 #define DIRSIZ 14
4664 
4665 struct dirent {
4666   ushort inum;
4667   char name[DIRSIZ];
4668 };
4669 
4670 
4671 
4672 
4673 
4674 
4675 
4676 
4677 
4678 
4679 
4680 
4681 
4682 
4683 
4684 
4685 
4686 
4687 
4688 
4689 
4690 
4691 
4692 
4693 
4694 
4695 
4696 
4697 
4698 
4699 
