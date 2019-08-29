4700 struct file {
4701   enum { FD_NONE, FD_PIPE, FD_INODE } type;
4702   int ref; // reference count
4703   char readable;
4704   char writable;
4705   struct pipe *pipe;
4706   struct inode *ip;
4707   uint off;
4708 };
4709 
4710 
4711 // in-memory copy of an inode
4712 struct inode {
4713   uint dev;           // Device number
4714   uint inum;          // Inode number
4715   int ref;            // Reference count
4716   struct sleeplock lock; // protects everything below here
4717   int valid;          // inode has been read from disk?
4718 
4719   short type;         // copy of disk inode
4720   short major;
4721   short minor;
4722   short nlink;
4723   uint size;
4724   uint addrs[NDIRECT+1];
4725 };
4726 
4727 // table mapping major device number to
4728 // device functions
4729 struct devsw {
4730   int (*read)(struct inode*, char*, int);
4731   int (*write)(struct inode*, char*, int);
4732 };
4733 
4734 extern struct devsw devsw[];
4735 
4736 #define CONSOLE 1
4737 
4738 
4739 
4740 
4741 
4742 
4743 
4744 
4745 
4746 
4747 
4748 
4749 
