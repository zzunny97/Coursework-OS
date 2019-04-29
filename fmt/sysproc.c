4100 #include "types.h"
4101 #include "x86.h"
4102 #include "defs.h"
4103 #include "date.h"
4104 #include "param.h"
4105 #include "memlayout.h"
4106 #include "mmu.h"
4107 #include "proc.h"
4108 #include "synch.h"
4109 
4110 int
4111 sys_fork(void)
4112 {
4113   return fork();
4114 }
4115 
4116 int
4117 sys_exit(void)
4118 {
4119   exit();
4120   return 0;  // not reached
4121 }
4122 
4123 int
4124 sys_wait(void)
4125 {
4126   return wait();
4127 }
4128 
4129 int
4130 sys_kill(void)
4131 {
4132   int pid;
4133 
4134   if(argint(0, &pid) < 0)
4135     return -1;
4136   return kill(pid);
4137 }
4138 
4139 int
4140 sys_getpid(void)
4141 {
4142   return myproc()->pid;
4143 }
4144 
4145 
4146 
4147 
4148 
4149 
4150 int
4151 sys_sbrk(void)
4152 {
4153   int addr;
4154   int n;
4155 
4156   if(argint(0, &n) < 0)
4157     return -1;
4158   addr = myproc()->sz;
4159   if(growproc(n) < 0)
4160     return -1;
4161   return addr;
4162 }
4163 
4164 int
4165 sys_sleep(void)
4166 {
4167   int n;
4168   uint ticks0;
4169 
4170   if(argint(0, &n) < 0)
4171     return -1;
4172   acquire(&tickslock);
4173   ticks0 = ticks;
4174   while(ticks - ticks0 < n){
4175     if(myproc()->killed){
4176       release(&tickslock);
4177       return -1;
4178     }
4179     sleep(&ticks, &tickslock);
4180   }
4181   release(&tickslock);
4182   return 0;
4183 }
4184 
4185 // return how many clock tick interrupts have occurred
4186 // since start.
4187 int
4188 sys_uptime(void)
4189 {
4190   uint xticks;
4191 
4192   acquire(&tickslock);
4193   xticks = ticks;
4194   release(&tickslock);
4195   return xticks;
4196 }
4197 
4198 
4199 
4200 int
4201 sys_halt(void)
4202 {
4203   outw(0x604, 0x2000);
4204   return 0;
4205 }
4206 
4207 int
4208 sys_getnice(void)
4209 {
4210   int pid;
4211 
4212   if(argint(0, &pid) < 0)
4213     return -1;
4214   return getnice(pid);
4215 }
4216 
4217 int
4218 sys_setnice(void)
4219 {
4220 	int pid, value;
4221 
4222 	if(argint(0, &pid) < 0)
4223 		return -1;
4224 	if(argint(1, &value) < 0)
4225 		return -1;
4226 	return setnice(pid, value);
4227 }
4228 
4229 void
4230 sys_ps(void)
4231 {
4232 	int pid;
4233 
4234 	if(argint(0, &pid) >= 0){
4235     ps(pid);
4236   }
4237 }
4238 
4239 
4240 
4241 
4242 
4243 
4244 
4245 
4246 
4247 
4248 
4249 
4250 int
4251 sys_thread_create(void)
4252 {
4253   int function, arg, stack;
4254   if(argint(0, &function) < 0)
4255     return -1;
4256   if(argint(1, &arg) < 0)
4257     return -1;
4258   if(argint(2, &stack) < 0)
4259     return -1;
4260   return thread_create((void*)function, (void*)arg, (void*)stack);
4261 }
4262 
4263 void
4264 sys_thread_exit(void)
4265 {
4266   int retval;
4267   if(argint(0, &retval) < 0)
4268     return;
4269   thread_exit((void*)retval);
4270 }
4271 
4272 int
4273 sys_thread_join(void)
4274 {
4275   int tid;
4276   int retval;
4277   if(argint(0, &tid) < 0)
4278     return -1;
4279   if(argint(1, &retval) < 0)
4280     return -1;
4281   return thread_join(tid, (void*)retval);
4282 }
4283 
4284 int
4285 sys_gettid(void)
4286 {
4287   int tid;
4288   if(argint(0, &tid) < 0)
4289     return -1;
4290   return gettid(tid);
4291 }
4292 
4293 
4294 
4295 
4296 
4297 
4298 
4299 
4300 int
4301 sys_mutex_init(void)
4302 {
4303   int mutex;
4304   if(argint(0, &mutex) < 0)
4305     return -1;
4306   return mutex_init((struct mutex_t*)mutex);
4307 }
4308 int
4309 sys_mutex_lock(void)
4310 {
4311   int mutex;
4312   if(argint(0, &mutex) < 0)
4313     return -1;
4314   return mutex_lock((struct mutex_t*)mutex);
4315 }
4316 
4317 int
4318 sys_mutex_unlock(void)
4319 {
4320   int mutex;
4321   if(argint(0, &mutex) < 0)
4322     return -1;
4323   return mutex_unlock((struct mutex_t*)mutex);
4324 }
4325 
4326 
4327 int
4328 sys_cond_init(void)
4329 {
4330   int cv;
4331   if(argint(0, &cv) < 0)
4332     return -1;
4333   return cond_init((struct cond_t*)cv);
4334 }
4335 
4336 int
4337 sys_cond_wait(void)
4338 {
4339   int mutex, cv;
4340   if(argint(0, &cv) < 0)
4341     return -1;
4342   if(argint(1, &mutex) < 0)
4343     return -1;
4344   return cond_wait((struct cond_t*)cv, (struct mutex_t*)mutex);
4345 }
4346 
4347 
4348 
4349 
4350 int
4351 sys_cond_signal(void)
4352 {
4353   int cv;
4354   if(argint(0, &cv) < 0)
4355     return -1;
4356   return cond_signal((struct cond_t*)cv);
4357 }
4358 
4359 
4360 
4361 
4362 
4363 
4364 
4365 
4366 
4367 
4368 
4369 
4370 
4371 
4372 
4373 
4374 
4375 
4376 
4377 
4378 
4379 
4380 
4381 
4382 
4383 
4384 
4385 
4386 
4387 
4388 
4389 
4390 
4391 
4392 
4393 
4394 
4395 
4396 
4397 
4398 
4399 
