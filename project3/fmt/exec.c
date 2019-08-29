7150 #include "types.h"
7151 #include "param.h"
7152 #include "memlayout.h"
7153 #include "mmu.h"
7154 #include "proc.h"
7155 #include "defs.h"
7156 #include "x86.h"
7157 #include "elf.h"
7158 
7159 int
7160 exec(char *path, char **argv)
7161 {
7162   char *s, *last;
7163   int i, off;
7164   uint argc, sz, sp, ustack[3+MAXARG+1];
7165   struct elfhdr elf;
7166   struct inode *ip;
7167   struct proghdr ph;
7168   pde_t *pgdir, *oldpgdir;
7169   struct proc *curproc = myproc();
7170 
7171   begin_op();
7172 
7173   if((ip = namei(path)) == 0){
7174     end_op();
7175     cprintf("exec: fail\n");
7176     return -1;
7177   }
7178   ilock(ip);
7179   pgdir = 0;
7180 
7181   // Check ELF header
7182   if(readi(ip, (char*)&elf, 0, sizeof(elf)) != sizeof(elf))
7183     goto bad;
7184   if(elf.magic != ELF_MAGIC)
7185     goto bad;
7186 
7187   if((pgdir = setupkvm()) == 0)
7188     goto bad;
7189 
7190   // Load program into memory.
7191   sz = 0;
7192   for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
7193     if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
7194       goto bad;
7195     if(ph.type != ELF_PROG_LOAD)
7196       continue;
7197     if(ph.memsz < ph.filesz)
7198       goto bad;
7199     if(ph.vaddr + ph.memsz < ph.vaddr)
7200       goto bad;
7201     if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
7202       goto bad;
7203     if(ph.vaddr % PGSIZE != 0)
7204       goto bad;
7205     if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
7206       goto bad;
7207   }
7208   iunlockput(ip);
7209   end_op();
7210   ip = 0;
7211 
7212   // Allocate two pages at the next page boundary.
7213   // Make the first inaccessible.  Use the second as the user stack.
7214   sz = PGROUNDUP(sz);
7215   if((sz = allocuvm(pgdir, sz, sz + 2*PGSIZE)) == 0)
7216     goto bad;
7217   clearpteu(pgdir, (char*)(sz - 2*PGSIZE));
7218   sp = sz;
7219 
7220   // Push argument strings, prepare rest of stack in ustack.
7221   for(argc = 0; argv[argc]; argc++) {
7222     if(argc >= MAXARG)
7223       goto bad;
7224     sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
7225     if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
7226       goto bad;
7227     ustack[3+argc] = sp;
7228   }
7229   ustack[3+argc] = 0;
7230 
7231   ustack[0] = 0xffffffff;  // fake return PC
7232   ustack[1] = argc;
7233   ustack[2] = sp - (argc+1)*4;  // argv pointer
7234 
7235   sp -= (3+argc+1) * 4;
7236   if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
7237     goto bad;
7238 
7239   // Save program name for debugging.
7240   for(last=s=path; *s; s++)
7241     if(*s == '/')
7242       last = s+1;
7243   safestrcpy(curproc->name, last, sizeof(curproc->name));
7244 
7245   // Commit to the user image.
7246   oldpgdir = curproc->pgdir;
7247   curproc->pgdir = pgdir;
7248   curproc->sz = sz;
7249   curproc->tf->eip = elf.entry;  // main
7250   curproc->tf->esp = sp;
7251   switchuvm(curproc);
7252   freevm(oldpgdir);
7253   return 0;
7254 
7255  bad:
7256   if(pgdir)
7257     freevm(pgdir);
7258   if(ip){
7259     iunlockput(ip);
7260     end_op();
7261   }
7262   return -1;
7263 }
7264 
7265 
7266 
7267 
7268 
7269 
7270 
7271 
7272 
7273 
7274 
7275 
7276 
7277 
7278 
7279 
7280 
7281 
7282 
7283 
7284 
7285 
7286 
7287 
7288 
7289 
7290 
7291 
7292 
7293 
7294 
7295 
7296 
7297 
7298 
7299 
