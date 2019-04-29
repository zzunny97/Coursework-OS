6600 //
6601 // File-system system calls.
6602 // Mostly argument checking, since we don't trust
6603 // user code, and calls into file.c and fs.c.
6604 //
6605 
6606 #include "types.h"
6607 #include "defs.h"
6608 #include "param.h"
6609 #include "stat.h"
6610 #include "mmu.h"
6611 #include "proc.h"
6612 #include "fs.h"
6613 #include "spinlock.h"
6614 #include "sleeplock.h"
6615 #include "file.h"
6616 #include "fcntl.h"
6617 
6618 // Fetch the nth word-sized system call argument as a file descriptor
6619 // and return both the descriptor and the corresponding struct file.
6620 static int
6621 argfd(int n, int *pfd, struct file **pf)
6622 {
6623   int fd;
6624   struct file *f;
6625 
6626   if(argint(n, &fd) < 0)
6627     return -1;
6628   if(fd < 0 || fd >= NOFILE || (f=myproc()->ofile[fd]) == 0)
6629     return -1;
6630   if(pfd)
6631     *pfd = fd;
6632   if(pf)
6633     *pf = f;
6634   return 0;
6635 }
6636 
6637 
6638 
6639 
6640 
6641 
6642 
6643 
6644 
6645 
6646 
6647 
6648 
6649 
6650 // Allocate a file descriptor for the given file.
6651 // Takes over file reference from caller on success.
6652 static int
6653 fdalloc(struct file *f)
6654 {
6655   int fd;
6656   struct proc *curproc = myproc();
6657 
6658   for(fd = 0; fd < NOFILE; fd++){
6659     if(curproc->ofile[fd] == 0){
6660       curproc->ofile[fd] = f;
6661       return fd;
6662     }
6663   }
6664   return -1;
6665 }
6666 
6667 int
6668 sys_dup(void)
6669 {
6670   struct file *f;
6671   int fd;
6672 
6673   if(argfd(0, 0, &f) < 0)
6674     return -1;
6675   if((fd=fdalloc(f)) < 0)
6676     return -1;
6677   filedup(f);
6678   return fd;
6679 }
6680 
6681 int
6682 sys_read(void)
6683 {
6684   struct file *f;
6685   int n;
6686   char *p;
6687 
6688   if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
6689     return -1;
6690   return fileread(f, p, n);
6691 }
6692 
6693 
6694 
6695 
6696 
6697 
6698 
6699 
6700 int
6701 sys_write(void)
6702 {
6703   struct file *f;
6704   int n;
6705   char *p;
6706 
6707   if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
6708     return -1;
6709   return filewrite(f, p, n);
6710 }
6711 
6712 int
6713 sys_close(void)
6714 {
6715   int fd;
6716   struct file *f;
6717 
6718   if(argfd(0, &fd, &f) < 0)
6719     return -1;
6720   myproc()->ofile[fd] = 0;
6721   fileclose(f);
6722   return 0;
6723 }
6724 
6725 int
6726 sys_fstat(void)
6727 {
6728   struct file *f;
6729   struct stat *st;
6730 
6731   if(argfd(0, 0, &f) < 0 || argptr(1, (void*)&st, sizeof(*st)) < 0)
6732     return -1;
6733   return filestat(f, st);
6734 }
6735 
6736 
6737 
6738 
6739 
6740 
6741 
6742 
6743 
6744 
6745 
6746 
6747 
6748 
6749 
6750 // Create the path new as a link to the same inode as old.
6751 int
6752 sys_link(void)
6753 {
6754   char name[DIRSIZ], *new, *old;
6755   struct inode *dp, *ip;
6756 
6757   if(argstr(0, &old) < 0 || argstr(1, &new) < 0)
6758     return -1;
6759 
6760   begin_op();
6761   if((ip = namei(old)) == 0){
6762     end_op();
6763     return -1;
6764   }
6765 
6766   ilock(ip);
6767   if(ip->type == T_DIR){
6768     iunlockput(ip);
6769     end_op();
6770     return -1;
6771   }
6772 
6773   ip->nlink++;
6774   iupdate(ip);
6775   iunlock(ip);
6776 
6777   if((dp = nameiparent(new, name)) == 0)
6778     goto bad;
6779   ilock(dp);
6780   if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
6781     iunlockput(dp);
6782     goto bad;
6783   }
6784   iunlockput(dp);
6785   iput(ip);
6786 
6787   end_op();
6788 
6789   return 0;
6790 
6791 bad:
6792   ilock(ip);
6793   ip->nlink--;
6794   iupdate(ip);
6795   iunlockput(ip);
6796   end_op();
6797   return -1;
6798 }
6799 
6800 // Is the directory dp empty except for "." and ".." ?
6801 static int
6802 isdirempty(struct inode *dp)
6803 {
6804   int off;
6805   struct dirent de;
6806 
6807   for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
6808     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
6809       panic("isdirempty: readi");
6810     if(de.inum != 0)
6811       return 0;
6812   }
6813   return 1;
6814 }
6815 
6816 
6817 
6818 
6819 
6820 
6821 
6822 
6823 
6824 
6825 
6826 
6827 
6828 
6829 
6830 
6831 
6832 
6833 
6834 
6835 
6836 
6837 
6838 
6839 
6840 
6841 
6842 
6843 
6844 
6845 
6846 
6847 
6848 
6849 
6850 int
6851 sys_unlink(void)
6852 {
6853   struct inode *ip, *dp;
6854   struct dirent de;
6855   char name[DIRSIZ], *path;
6856   uint off;
6857 
6858   if(argstr(0, &path) < 0)
6859     return -1;
6860 
6861   begin_op();
6862   if((dp = nameiparent(path, name)) == 0){
6863     end_op();
6864     return -1;
6865   }
6866 
6867   ilock(dp);
6868 
6869   // Cannot unlink "." or "..".
6870   if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0)
6871     goto bad;
6872 
6873   if((ip = dirlookup(dp, name, &off)) == 0)
6874     goto bad;
6875   ilock(ip);
6876 
6877   if(ip->nlink < 1)
6878     panic("unlink: nlink < 1");
6879   if(ip->type == T_DIR && !isdirempty(ip)){
6880     iunlockput(ip);
6881     goto bad;
6882   }
6883 
6884   memset(&de, 0, sizeof(de));
6885   if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
6886     panic("unlink: writei");
6887   if(ip->type == T_DIR){
6888     dp->nlink--;
6889     iupdate(dp);
6890   }
6891   iunlockput(dp);
6892 
6893   ip->nlink--;
6894   iupdate(ip);
6895   iunlockput(ip);
6896 
6897   end_op();
6898 
6899   return 0;
6900 bad:
6901   iunlockput(dp);
6902   end_op();
6903   return -1;
6904 }
6905 
6906 static struct inode*
6907 create(char *path, short type, short major, short minor)
6908 {
6909   uint off;
6910   struct inode *ip, *dp;
6911   char name[DIRSIZ];
6912 
6913   if((dp = nameiparent(path, name)) == 0)
6914     return 0;
6915   ilock(dp);
6916 
6917   if((ip = dirlookup(dp, name, &off)) != 0){
6918     iunlockput(dp);
6919     ilock(ip);
6920     if(type == T_FILE && ip->type == T_FILE)
6921       return ip;
6922     iunlockput(ip);
6923     return 0;
6924   }
6925 
6926   if((ip = ialloc(dp->dev, type)) == 0)
6927     panic("create: ialloc");
6928 
6929   ilock(ip);
6930   ip->major = major;
6931   ip->minor = minor;
6932   ip->nlink = 1;
6933   iupdate(ip);
6934 
6935   if(type == T_DIR){  // Create . and .. entries.
6936     dp->nlink++;  // for ".."
6937     iupdate(dp);
6938     // No ip->nlink++ for ".": avoid cyclic ref count.
6939     if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0)
6940       panic("create dots");
6941   }
6942 
6943   if(dirlink(dp, name, ip->inum) < 0)
6944     panic("create: dirlink");
6945 
6946   iunlockput(dp);
6947 
6948   return ip;
6949 }
6950 int
6951 sys_open(void)
6952 {
6953   char *path;
6954   int fd, omode;
6955   struct file *f;
6956   struct inode *ip;
6957 
6958   if(argstr(0, &path) < 0 || argint(1, &omode) < 0)
6959     return -1;
6960 
6961   begin_op();
6962 
6963   if(omode & O_CREATE){
6964     ip = create(path, T_FILE, 0, 0);
6965     if(ip == 0){
6966       end_op();
6967       return -1;
6968     }
6969   } else {
6970     if((ip = namei(path)) == 0){
6971       end_op();
6972       return -1;
6973     }
6974     ilock(ip);
6975     if(ip->type == T_DIR && omode != O_RDONLY){
6976       iunlockput(ip);
6977       end_op();
6978       return -1;
6979     }
6980   }
6981 
6982   if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
6983     if(f)
6984       fileclose(f);
6985     iunlockput(ip);
6986     end_op();
6987     return -1;
6988   }
6989   iunlock(ip);
6990   end_op();
6991 
6992   f->type = FD_INODE;
6993   f->ip = ip;
6994   f->off = 0;
6995   f->readable = !(omode & O_WRONLY);
6996   f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
6997   return fd;
6998 }
6999 
7000 int
7001 sys_mkdir(void)
7002 {
7003   char *path;
7004   struct inode *ip;
7005 
7006   begin_op();
7007   if(argstr(0, &path) < 0 || (ip = create(path, T_DIR, 0, 0)) == 0){
7008     end_op();
7009     return -1;
7010   }
7011   iunlockput(ip);
7012   end_op();
7013   return 0;
7014 }
7015 
7016 int
7017 sys_mknod(void)
7018 {
7019   struct inode *ip;
7020   char *path;
7021   int major, minor;
7022 
7023   begin_op();
7024   if((argstr(0, &path)) < 0 ||
7025      argint(1, &major) < 0 ||
7026      argint(2, &minor) < 0 ||
7027      (ip = create(path, T_DEV, major, minor)) == 0){
7028     end_op();
7029     return -1;
7030   }
7031   iunlockput(ip);
7032   end_op();
7033   return 0;
7034 }
7035 
7036 
7037 
7038 
7039 
7040 
7041 
7042 
7043 
7044 
7045 
7046 
7047 
7048 
7049 
7050 int
7051 sys_chdir(void)
7052 {
7053   char *path;
7054   struct inode *ip;
7055   struct proc *curproc = myproc();
7056 
7057   begin_op();
7058   if(argstr(0, &path) < 0 || (ip = namei(path)) == 0){
7059     end_op();
7060     return -1;
7061   }
7062   ilock(ip);
7063   if(ip->type != T_DIR){
7064     iunlockput(ip);
7065     end_op();
7066     return -1;
7067   }
7068   iunlock(ip);
7069   iput(curproc->cwd);
7070   end_op();
7071   curproc->cwd = ip;
7072   return 0;
7073 }
7074 
7075 int
7076 sys_exec(void)
7077 {
7078   char *path, *argv[MAXARG];
7079   int i;
7080   uint uargv, uarg;
7081 
7082   if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0){
7083     return -1;
7084   }
7085   memset(argv, 0, sizeof(argv));
7086   for(i=0;; i++){
7087     if(i >= NELEM(argv))
7088       return -1;
7089     if(fetchint(uargv+4*i, (int*)&uarg) < 0)
7090       return -1;
7091     if(uarg == 0){
7092       argv[i] = 0;
7093       break;
7094     }
7095     if(fetchstr(uarg, &argv[i]) < 0)
7096       return -1;
7097   }
7098   return exec(path, argv);
7099 }
7100 int
7101 sys_pipe(void)
7102 {
7103   int *fd;
7104   struct file *rf, *wf;
7105   int fd0, fd1;
7106 
7107   if(argptr(0, (void*)&fd, 2*sizeof(fd[0])) < 0)
7108     return -1;
7109   if(pipealloc(&rf, &wf) < 0)
7110     return -1;
7111   fd0 = -1;
7112   if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
7113     if(fd0 >= 0)
7114       myproc()->ofile[fd0] = 0;
7115     fileclose(rf);
7116     fileclose(wf);
7117     return -1;
7118   }
7119   fd[0] = fd0;
7120   fd[1] = fd1;
7121   return 0;
7122 }
7123 
7124 
7125 
7126 
7127 
7128 
7129 
7130 
7131 
7132 
7133 
7134 
7135 
7136 
7137 
7138 
7139 
7140 
7141 
7142 
7143 
7144 
7145 
7146 
7147 
7148 
7149 
