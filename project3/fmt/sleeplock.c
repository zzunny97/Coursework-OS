5150 // Sleeping locks
5151 
5152 #include "types.h"
5153 #include "defs.h"
5154 #include "param.h"
5155 #include "x86.h"
5156 #include "memlayout.h"
5157 #include "mmu.h"
5158 #include "proc.h"
5159 #include "spinlock.h"
5160 #include "sleeplock.h"
5161 
5162 void
5163 initsleeplock(struct sleeplock *lk, char *name)
5164 {
5165   initlock(&lk->lk, "sleep lock");
5166   lk->name = name;
5167   lk->locked = 0;
5168   lk->pid = 0;
5169 }
5170 
5171 void
5172 acquiresleep(struct sleeplock *lk)
5173 {
5174   acquire(&lk->lk);
5175   while (lk->locked) {
5176     sleep(lk, &lk->lk);
5177   }
5178   lk->locked = 1;
5179   lk->pid = myproc()->pid;
5180   release(&lk->lk);
5181 }
5182 
5183 void
5184 releasesleep(struct sleeplock *lk)
5185 {
5186   acquire(&lk->lk);
5187   lk->locked = 0;
5188   lk->pid = 0;
5189   wakeup(lk);
5190   release(&lk->lk);
5191 }
5192 
5193 
5194 
5195 
5196 
5197 
5198 
5199 
5200 int
5201 holdingsleep(struct sleeplock *lk)
5202 {
5203   int r;
5204 
5205   acquire(&lk->lk);
5206   r = lk->locked && (lk->pid == myproc()->pid);
5207   release(&lk->lk);
5208   return r;
5209 }
5210 
5211 
5212 
5213 
5214 
5215 
5216 
5217 
5218 
5219 
5220 
5221 
5222 
5223 
5224 
5225 
5226 
5227 
5228 
5229 
5230 
5231 
5232 
5233 
5234 
5235 
5236 
5237 
5238 
5239 
5240 
5241 
5242 
5243 
5244 
5245 
5246 
5247 
5248 
5249 
