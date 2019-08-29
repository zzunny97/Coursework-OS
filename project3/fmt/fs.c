5500 // File system implementation.  Five layers:
5501 //   + Blocks: allocator for raw disk blocks.
5502 //   + Log: crash recovery for multi-step updates.
5503 //   + Files: inode allocator, reading, writing, metadata.
5504 //   + Directories: inode with special contents (list of other inodes!)
5505 //   + Names: paths like /usr/rtm/xv6/fs.c for convenient naming.
5506 //
5507 // This file contains the low-level file system manipulation
5508 // routines.  The (higher-level) system call implementations
5509 // are in sysfile.c.
5510 
5511 #include "types.h"
5512 #include "defs.h"
5513 #include "param.h"
5514 #include "stat.h"
5515 #include "mmu.h"
5516 #include "proc.h"
5517 #include "spinlock.h"
5518 #include "sleeplock.h"
5519 #include "fs.h"
5520 #include "buf.h"
5521 #include "file.h"
5522 
5523 #define min(a, b) ((a) < (b) ? (a) : (b))
5524 static void itrunc(struct inode*);
5525 // there should be one superblock per disk device, but we run with
5526 // only one device
5527 struct superblock sb;
5528 
5529 // Read the super block.
5530 void
5531 readsb(int dev, struct superblock *sb)
5532 {
5533   struct buf *bp;
5534 
5535   bp = bread(dev, 1);
5536   memmove(sb, bp->data, sizeof(*sb));
5537   brelse(bp);
5538 }
5539 
5540 
5541 
5542 
5543 
5544 
5545 
5546 
5547 
5548 
5549 
5550 // Zero a block.
5551 static void
5552 bzero(int dev, int bno)
5553 {
5554   struct buf *bp;
5555 
5556   bp = bread(dev, bno);
5557   memset(bp->data, 0, BSIZE);
5558   log_write(bp);
5559   brelse(bp);
5560 }
5561 
5562 // Blocks.
5563 
5564 // Allocate a zeroed disk block.
5565 static uint
5566 balloc(uint dev)
5567 {
5568   int b, bi, m;
5569   struct buf *bp;
5570 
5571   bp = 0;
5572   for(b = 0; b < sb.size; b += BPB){
5573     bp = bread(dev, BBLOCK(b, sb));
5574     for(bi = 0; bi < BPB && b + bi < sb.size; bi++){
5575       m = 1 << (bi % 8);
5576       if((bp->data[bi/8] & m) == 0){  // Is block free?
5577         bp->data[bi/8] |= m;  // Mark block in use.
5578         log_write(bp);
5579         brelse(bp);
5580         bzero(dev, b + bi);
5581         return b + bi;
5582       }
5583     }
5584     brelse(bp);
5585   }
5586   panic("balloc: out of blocks");
5587 }
5588 
5589 
5590 
5591 
5592 
5593 
5594 
5595 
5596 
5597 
5598 
5599 
5600 // Free a disk block.
5601 static void
5602 bfree(int dev, uint b)
5603 {
5604   struct buf *bp;
5605   int bi, m;
5606 
5607   readsb(dev, &sb);
5608   bp = bread(dev, BBLOCK(b, sb));
5609   bi = b % BPB;
5610   m = 1 << (bi % 8);
5611   if((bp->data[bi/8] & m) == 0)
5612     panic("freeing free block");
5613   bp->data[bi/8] &= ~m;
5614   log_write(bp);
5615   brelse(bp);
5616 }
5617 
5618 // Inodes.
5619 //
5620 // An inode describes a single unnamed file.
5621 // The inode disk structure holds metadata: the file's type,
5622 // its size, the number of links referring to it, and the
5623 // list of blocks holding the file's content.
5624 //
5625 // The inodes are laid out sequentially on disk at
5626 // sb.startinode. Each inode has a number, indicating its
5627 // position on the disk.
5628 //
5629 // The kernel keeps a cache of in-use inodes in memory
5630 // to provide a place for synchronizing access
5631 // to inodes used by multiple processes. The cached
5632 // inodes include book-keeping information that is
5633 // not stored on disk: ip->ref and ip->valid.
5634 //
5635 // An inode and its in-memory representation go through a
5636 // sequence of states before they can be used by the
5637 // rest of the file system code.
5638 //
5639 // * Allocation: an inode is allocated if its type (on disk)
5640 //   is non-zero. ialloc() allocates, and iput() frees if
5641 //   the reference and link counts have fallen to zero.
5642 //
5643 // * Referencing in cache: an entry in the inode cache
5644 //   is free if ip->ref is zero. Otherwise ip->ref tracks
5645 //   the number of in-memory pointers to the entry (open
5646 //   files and current directories). iget() finds or
5647 //   creates a cache entry and increments its ref; iput()
5648 //   decrements ref.
5649 //
5650 // * Valid: the information (type, size, &c) in an inode
5651 //   cache entry is only correct when ip->valid is 1.
5652 //   ilock() reads the inode from
5653 //   the disk and sets ip->valid, while iput() clears
5654 //   ip->valid if ip->ref has fallen to zero.
5655 //
5656 // * Locked: file system code may only examine and modify
5657 //   the information in an inode and its content if it
5658 //   has first locked the inode.
5659 //
5660 // Thus a typical sequence is:
5661 //   ip = iget(dev, inum)
5662 //   ilock(ip)
5663 //   ... examine and modify ip->xxx ...
5664 //   iunlock(ip)
5665 //   iput(ip)
5666 //
5667 // ilock() is separate from iget() so that system calls can
5668 // get a long-term reference to an inode (as for an open file)
5669 // and only lock it for short periods (e.g., in read()).
5670 // The separation also helps avoid deadlock and races during
5671 // pathname lookup. iget() increments ip->ref so that the inode
5672 // stays cached and pointers to it remain valid.
5673 //
5674 // Many internal file system functions expect the caller to
5675 // have locked the inodes involved; this lets callers create
5676 // multi-step atomic operations.
5677 //
5678 // The icache.lock spin-lock protects the allocation of icache
5679 // entries. Since ip->ref indicates whether an entry is free,
5680 // and ip->dev and ip->inum indicate which i-node an entry
5681 // holds, one must hold icache.lock while using any of those fields.
5682 //
5683 // An ip->lock sleep-lock protects all ip-> fields other than ref,
5684 // dev, and inum.  One must hold ip->lock in order to
5685 // read or write that inode's ip->valid, ip->size, ip->type, &c.
5686 
5687 struct {
5688   struct spinlock lock;
5689   struct inode inode[NINODE];
5690 } icache;
5691 
5692 void
5693 iinit(int dev)
5694 {
5695   int i = 0;
5696 
5697   initlock(&icache.lock, "icache");
5698   for(i = 0; i < NINODE; i++) {
5699     initsleeplock(&icache.inode[i].lock, "inode");
5700   }
5701 
5702   readsb(dev, &sb);
5703   cprintf("sb: size %d nblocks %d ninodes %d nlog %d logstart %d\
5704  inodestart %d bmap start %d\n", sb.size, sb.nblocks,
5705           sb.ninodes, sb.nlog, sb.logstart, sb.inodestart,
5706           sb.bmapstart);
5707 }
5708 
5709 static struct inode* iget(uint dev, uint inum);
5710 
5711 
5712 
5713 
5714 
5715 
5716 
5717 
5718 
5719 
5720 
5721 
5722 
5723 
5724 
5725 
5726 
5727 
5728 
5729 
5730 
5731 
5732 
5733 
5734 
5735 
5736 
5737 
5738 
5739 
5740 
5741 
5742 
5743 
5744 
5745 
5746 
5747 
5748 
5749 
5750 // Allocate an inode on device dev.
5751 // Mark it as allocated by  giving it type type.
5752 // Returns an unlocked but allocated and referenced inode.
5753 struct inode*
5754 ialloc(uint dev, short type)
5755 {
5756   int inum;
5757   struct buf *bp;
5758   struct dinode *dip;
5759 
5760   for(inum = 1; inum < sb.ninodes; inum++){
5761     bp = bread(dev, IBLOCK(inum, sb));
5762     dip = (struct dinode*)bp->data + inum%IPB;
5763     if(dip->type == 0){  // a free inode
5764       memset(dip, 0, sizeof(*dip));
5765       dip->type = type;
5766       log_write(bp);   // mark it allocated on the disk
5767       brelse(bp);
5768       return iget(dev, inum);
5769     }
5770     brelse(bp);
5771   }
5772   panic("ialloc: no inodes");
5773 }
5774 
5775 // Copy a modified in-memory inode to disk.
5776 // Must be called after every change to an ip->xxx field
5777 // that lives on disk, since i-node cache is write-through.
5778 // Caller must hold ip->lock.
5779 void
5780 iupdate(struct inode *ip)
5781 {
5782   struct buf *bp;
5783   struct dinode *dip;
5784 
5785   bp = bread(ip->dev, IBLOCK(ip->inum, sb));
5786   dip = (struct dinode*)bp->data + ip->inum%IPB;
5787   dip->type = ip->type;
5788   dip->major = ip->major;
5789   dip->minor = ip->minor;
5790   dip->nlink = ip->nlink;
5791   dip->size = ip->size;
5792   memmove(dip->addrs, ip->addrs, sizeof(ip->addrs));
5793   log_write(bp);
5794   brelse(bp);
5795 }
5796 
5797 
5798 
5799 
5800 // Find the inode with number inum on device dev
5801 // and return the in-memory copy. Does not lock
5802 // the inode and does not read it from disk.
5803 static struct inode*
5804 iget(uint dev, uint inum)
5805 {
5806   struct inode *ip, *empty;
5807 
5808   acquire(&icache.lock);
5809 
5810   // Is the inode already cached?
5811   empty = 0;
5812   for(ip = &icache.inode[0]; ip < &icache.inode[NINODE]; ip++){
5813     if(ip->ref > 0 && ip->dev == dev && ip->inum == inum){
5814       ip->ref++;
5815       release(&icache.lock);
5816       return ip;
5817     }
5818     if(empty == 0 && ip->ref == 0)    // Remember empty slot.
5819       empty = ip;
5820   }
5821 
5822   // Recycle an inode cache entry.
5823   if(empty == 0)
5824     panic("iget: no inodes");
5825 
5826   ip = empty;
5827   ip->dev = dev;
5828   ip->inum = inum;
5829   ip->ref = 1;
5830   ip->valid = 0;
5831   release(&icache.lock);
5832 
5833   return ip;
5834 }
5835 
5836 // Increment reference count for ip.
5837 // Returns ip to enable ip = idup(ip1) idiom.
5838 struct inode*
5839 idup(struct inode *ip)
5840 {
5841   acquire(&icache.lock);
5842   ip->ref++;
5843   release(&icache.lock);
5844   return ip;
5845 }
5846 
5847 
5848 
5849 
5850 // Lock the given inode.
5851 // Reads the inode from disk if necessary.
5852 void
5853 ilock(struct inode *ip)
5854 {
5855   struct buf *bp;
5856   struct dinode *dip;
5857 
5858   if(ip == 0 || ip->ref < 1)
5859     panic("ilock");
5860 
5861   acquiresleep(&ip->lock);
5862 
5863   if(ip->valid == 0){
5864     bp = bread(ip->dev, IBLOCK(ip->inum, sb));
5865     dip = (struct dinode*)bp->data + ip->inum%IPB;
5866     ip->type = dip->type;
5867     ip->major = dip->major;
5868     ip->minor = dip->minor;
5869     ip->nlink = dip->nlink;
5870     ip->size = dip->size;
5871     memmove(ip->addrs, dip->addrs, sizeof(ip->addrs));
5872     brelse(bp);
5873     ip->valid = 1;
5874     if(ip->type == 0)
5875       panic("ilock: no type");
5876   }
5877 }
5878 
5879 // Unlock the given inode.
5880 void
5881 iunlock(struct inode *ip)
5882 {
5883   if(ip == 0 || !holdingsleep(&ip->lock) || ip->ref < 1)
5884     panic("iunlock");
5885 
5886   releasesleep(&ip->lock);
5887 }
5888 
5889 
5890 
5891 
5892 
5893 
5894 
5895 
5896 
5897 
5898 
5899 
5900 // Drop a reference to an in-memory inode.
5901 // If that was the last reference, the inode cache entry can
5902 // be recycled.
5903 // If that was the last reference and the inode has no links
5904 // to it, free the inode (and its content) on disk.
5905 // All calls to iput() must be inside a transaction in
5906 // case it has to free the inode.
5907 void
5908 iput(struct inode *ip)
5909 {
5910   acquiresleep(&ip->lock);
5911   if(ip->valid && ip->nlink == 0){
5912     acquire(&icache.lock);
5913     int r = ip->ref;
5914     release(&icache.lock);
5915     if(r == 1){
5916       // inode has no links and no other references: truncate and free.
5917       itrunc(ip);
5918       ip->type = 0;
5919       iupdate(ip);
5920       ip->valid = 0;
5921     }
5922   }
5923   releasesleep(&ip->lock);
5924 
5925   acquire(&icache.lock);
5926   ip->ref--;
5927   release(&icache.lock);
5928 }
5929 
5930 // Common idiom: unlock, then put.
5931 void
5932 iunlockput(struct inode *ip)
5933 {
5934   iunlock(ip);
5935   iput(ip);
5936 }
5937 
5938 
5939 
5940 
5941 
5942 
5943 
5944 
5945 
5946 
5947 
5948 
5949 
5950 // Inode content
5951 //
5952 // The content (data) associated with each inode is stored
5953 // in blocks on the disk. The first NDIRECT block numbers
5954 // are listed in ip->addrs[].  The next NINDIRECT blocks are
5955 // listed in block ip->addrs[NDIRECT].
5956 
5957 // Return the disk block address of the nth block in inode ip.
5958 // If there is no such block, bmap allocates one.
5959 static uint
5960 bmap(struct inode *ip, uint bn)
5961 {
5962   uint addr, *a;
5963   struct buf *bp;
5964 
5965   if(bn < NDIRECT){
5966     if((addr = ip->addrs[bn]) == 0)
5967       ip->addrs[bn] = addr = balloc(ip->dev);
5968     return addr;
5969   }
5970   bn -= NDIRECT;
5971 
5972   if(bn < NINDIRECT){
5973     // Load indirect block, allocating if necessary.
5974     if((addr = ip->addrs[NDIRECT]) == 0)
5975       ip->addrs[NDIRECT] = addr = balloc(ip->dev);
5976     bp = bread(ip->dev, addr);
5977     a = (uint*)bp->data;
5978     if((addr = a[bn]) == 0){
5979       a[bn] = addr = balloc(ip->dev);
5980       log_write(bp);
5981     }
5982     brelse(bp);
5983     return addr;
5984   }
5985 
5986   panic("bmap: out of range");
5987 }
5988 
5989 
5990 
5991 
5992 
5993 
5994 
5995 
5996 
5997 
5998 
5999 
6000 // Truncate inode (discard contents).
6001 // Only called when the inode has no links
6002 // to it (no directory entries referring to it)
6003 // and has no in-memory reference to it (is
6004 // not an open file or current directory).
6005 static void
6006 itrunc(struct inode *ip)
6007 {
6008   int i, j;
6009   struct buf *bp;
6010   uint *a;
6011 
6012   for(i = 0; i < NDIRECT; i++){
6013     if(ip->addrs[i]){
6014       bfree(ip->dev, ip->addrs[i]);
6015       ip->addrs[i] = 0;
6016     }
6017   }
6018 
6019   if(ip->addrs[NDIRECT]){
6020     bp = bread(ip->dev, ip->addrs[NDIRECT]);
6021     a = (uint*)bp->data;
6022     for(j = 0; j < NINDIRECT; j++){
6023       if(a[j])
6024         bfree(ip->dev, a[j]);
6025     }
6026     brelse(bp);
6027     bfree(ip->dev, ip->addrs[NDIRECT]);
6028     ip->addrs[NDIRECT] = 0;
6029   }
6030 
6031   ip->size = 0;
6032   iupdate(ip);
6033 }
6034 
6035 // Copy stat information from inode.
6036 // Caller must hold ip->lock.
6037 void
6038 stati(struct inode *ip, struct stat *st)
6039 {
6040   st->dev = ip->dev;
6041   st->ino = ip->inum;
6042   st->type = ip->type;
6043   st->nlink = ip->nlink;
6044   st->size = ip->size;
6045 }
6046 
6047 
6048 
6049 
6050 // Read data from inode.
6051 // Caller must hold ip->lock.
6052 int
6053 readi(struct inode *ip, char *dst, uint off, uint n)
6054 {
6055   uint tot, m;
6056   struct buf *bp;
6057 
6058   if(ip->type == T_DEV){
6059     if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].read)
6060       return -1;
6061     return devsw[ip->major].read(ip, dst, n);
6062   }
6063 
6064   if(off > ip->size || off + n < off)
6065     return -1;
6066   if(off + n > ip->size)
6067     n = ip->size - off;
6068 
6069   for(tot=0; tot<n; tot+=m, off+=m, dst+=m){
6070     bp = bread(ip->dev, bmap(ip, off/BSIZE));
6071     m = min(n - tot, BSIZE - off%BSIZE);
6072     memmove(dst, bp->data + off%BSIZE, m);
6073     brelse(bp);
6074   }
6075   return n;
6076 }
6077 
6078 
6079 
6080 
6081 
6082 
6083 
6084 
6085 
6086 
6087 
6088 
6089 
6090 
6091 
6092 
6093 
6094 
6095 
6096 
6097 
6098 
6099 
6100 // Write data to inode.
6101 // Caller must hold ip->lock.
6102 int
6103 writei(struct inode *ip, char *src, uint off, uint n)
6104 {
6105   uint tot, m;
6106   struct buf *bp;
6107 
6108   if(ip->type == T_DEV){
6109     if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].write)
6110       return -1;
6111     return devsw[ip->major].write(ip, src, n);
6112   }
6113 
6114   if(off > ip->size || off + n < off)
6115     return -1;
6116   if(off + n > MAXFILE*BSIZE)
6117     return -1;
6118 
6119   for(tot=0; tot<n; tot+=m, off+=m, src+=m){
6120     bp = bread(ip->dev, bmap(ip, off/BSIZE));
6121     m = min(n - tot, BSIZE - off%BSIZE);
6122     memmove(bp->data + off%BSIZE, src, m);
6123     log_write(bp);
6124     brelse(bp);
6125   }
6126 
6127   if(n > 0 && off > ip->size){
6128     ip->size = off;
6129     iupdate(ip);
6130   }
6131   return n;
6132 }
6133 
6134 
6135 
6136 
6137 
6138 
6139 
6140 
6141 
6142 
6143 
6144 
6145 
6146 
6147 
6148 
6149 
6150 // Directories
6151 
6152 int
6153 namecmp(const char *s, const char *t)
6154 {
6155   return strncmp(s, t, DIRSIZ);
6156 }
6157 
6158 // Look for a directory entry in a directory.
6159 // If found, set *poff to byte offset of entry.
6160 struct inode*
6161 dirlookup(struct inode *dp, char *name, uint *poff)
6162 {
6163   uint off, inum;
6164   struct dirent de;
6165 
6166   if(dp->type != T_DIR)
6167     panic("dirlookup not DIR");
6168 
6169   for(off = 0; off < dp->size; off += sizeof(de)){
6170     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
6171       panic("dirlookup read");
6172     if(de.inum == 0)
6173       continue;
6174     if(namecmp(name, de.name) == 0){
6175       // entry matches path element
6176       if(poff)
6177         *poff = off;
6178       inum = de.inum;
6179       return iget(dp->dev, inum);
6180     }
6181   }
6182 
6183   return 0;
6184 }
6185 
6186 
6187 
6188 
6189 
6190 
6191 
6192 
6193 
6194 
6195 
6196 
6197 
6198 
6199 
6200 // Write a new directory entry (name, inum) into the directory dp.
6201 int
6202 dirlink(struct inode *dp, char *name, uint inum)
6203 {
6204   int off;
6205   struct dirent de;
6206   struct inode *ip;
6207 
6208   // Check that name is not present.
6209   if((ip = dirlookup(dp, name, 0)) != 0){
6210     iput(ip);
6211     return -1;
6212   }
6213 
6214   // Look for an empty dirent.
6215   for(off = 0; off < dp->size; off += sizeof(de)){
6216     if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
6217       panic("dirlink read");
6218     if(de.inum == 0)
6219       break;
6220   }
6221 
6222   strncpy(de.name, name, DIRSIZ);
6223   de.inum = inum;
6224   if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
6225     panic("dirlink");
6226 
6227   return 0;
6228 }
6229 
6230 
6231 
6232 
6233 
6234 
6235 
6236 
6237 
6238 
6239 
6240 
6241 
6242 
6243 
6244 
6245 
6246 
6247 
6248 
6249 
6250 // Paths
6251 
6252 // Copy the next path element from path into name.
6253 // Return a pointer to the element following the copied one.
6254 // The returned path has no leading slashes,
6255 // so the caller can check *path=='\0' to see if the name is the last one.
6256 // If no name to remove, return 0.
6257 //
6258 // Examples:
6259 //   skipelem("a/bb/c", name) = "bb/c", setting name = "a"
6260 //   skipelem("///a//bb", name) = "bb", setting name = "a"
6261 //   skipelem("a", name) = "", setting name = "a"
6262 //   skipelem("", name) = skipelem("////", name) = 0
6263 //
6264 static char*
6265 skipelem(char *path, char *name)
6266 {
6267   char *s;
6268   int len;
6269 
6270   while(*path == '/')
6271     path++;
6272   if(*path == 0)
6273     return 0;
6274   s = path;
6275   while(*path != '/' && *path != 0)
6276     path++;
6277   len = path - s;
6278   if(len >= DIRSIZ)
6279     memmove(name, s, DIRSIZ);
6280   else {
6281     memmove(name, s, len);
6282     name[len] = 0;
6283   }
6284   while(*path == '/')
6285     path++;
6286   return path;
6287 }
6288 
6289 
6290 
6291 
6292 
6293 
6294 
6295 
6296 
6297 
6298 
6299 
6300 // Look up and return the inode for a path name.
6301 // If parent != 0, return the inode for the parent and copy the final
6302 // path element into name, which must have room for DIRSIZ bytes.
6303 // Must be called inside a transaction since it calls iput().
6304 static struct inode*
6305 namex(char *path, int nameiparent, char *name)
6306 {
6307   struct inode *ip, *next;
6308 
6309   if(*path == '/')
6310     ip = iget(ROOTDEV, ROOTINO);
6311   else
6312     ip = idup(myproc()->cwd);
6313 
6314   while((path = skipelem(path, name)) != 0){
6315     ilock(ip);
6316     if(ip->type != T_DIR){
6317       iunlockput(ip);
6318       return 0;
6319     }
6320     if(nameiparent && *path == '\0'){
6321       // Stop one level early.
6322       iunlock(ip);
6323       return ip;
6324     }
6325     if((next = dirlookup(ip, name, 0)) == 0){
6326       iunlockput(ip);
6327       return 0;
6328     }
6329     iunlockput(ip);
6330     ip = next;
6331   }
6332   if(nameiparent){
6333     iput(ip);
6334     return 0;
6335   }
6336   return ip;
6337 }
6338 
6339 struct inode*
6340 namei(char *path)
6341 {
6342   char name[DIRSIZ];
6343   return namex(path, 0, name);
6344 }
6345 
6346 
6347 
6348 
6349 
6350 struct inode*
6351 nameiparent(char *path, char *name)
6352 {
6353   return namex(path, 1, name);
6354 }
6355 
6356 
6357 
6358 
6359 
6360 
6361 
6362 
6363 
6364 
6365 
6366 
6367 
6368 
6369 
6370 
6371 
6372 
6373 
6374 
6375 
6376 
6377 
6378 
6379 
6380 
6381 
6382 
6383 
6384 
6385 
6386 
6387 
6388 
6389 
6390 
6391 
6392 
6393 
6394 
6395 
6396 
6397 
6398 
6399 
