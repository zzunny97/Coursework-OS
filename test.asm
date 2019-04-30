
_test:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:
  thread_exit(0);
}

  int
main(int argc, char **argv)
{
   0:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   4:	83 e4 f0             	and    $0xfffffff0,%esp
   7:	ff 71 fc             	pushl  -0x4(%ecx)
   a:	55                   	push   %ebp
   b:	89 e5                	mov    %esp,%ebp
   d:	57                   	push   %edi
   e:	56                   	push   %esi
   f:	53                   	push   %ebx
  10:	51                   	push   %ecx
  11:	be a0 0e 00 00       	mov    $0xea0,%esi
  16:	bf c0 0e 00 00       	mov    $0xec0,%edi
  int i;

  printf(1, "TEST: ");
  1b:	89 f3                	mov    %esi,%ebx
  thread_exit(0);
}

  int
main(int argc, char **argv)
{
  1d:	83 ec 10             	sub    $0x10,%esp
  int i;

  printf(1, "TEST: ");
  20:	68 70 0a 00 00       	push   $0xa70
  25:	6a 01                	push   $0x1
  27:	e8 c4 06 00 00       	call   6f0 <printf>
  2c:	83 c4 10             	add    $0x10,%esp
  2f:	90                   	nop

  for(i=0;i<NTHREAD;i++)
    stack[i] = malloc(4096);
  30:	83 ec 0c             	sub    $0xc,%esp
  33:	83 c3 04             	add    $0x4,%ebx
  36:	68 00 10 00 00       	push   $0x1000
  3b:	e8 e0 08 00 00       	call   920 <malloc>
  40:	89 43 fc             	mov    %eax,-0x4(%ebx)
{
  int i;

  printf(1, "TEST: ");

  for(i=0;i<NTHREAD;i++)
  43:	83 c4 10             	add    $0x10,%esp
  46:	39 df                	cmp    %ebx,%edi
  48:	75 e6                	jne    30 <main+0x30>
    stack[i] = malloc(4096);
  mutex_init(&m);
  4a:	83 ec 0c             	sub    $0xc,%esp
  4d:	68 c0 0f 00 00       	push   $0xfc0
  52:	e8 bb 05 00 00       	call   612 <mutex_init>
  cond_init(&c);
  57:	c7 04 24 20 0f 00 00 	movl   $0xf20,(%esp)
  5e:	e8 c7 05 00 00       	call   62a <cond_init>
  printf(1, "\nmutex and cv init complete\n");
  63:	5b                   	pop    %ebx
  64:	58                   	pop    %eax
  65:	68 77 0a 00 00       	push   $0xa77
  6a:	6a 01                	push   $0x1
  for(i=0;i<NTHREAD-1;i++){
  6c:	31 db                	xor    %ebx,%ebx

  for(i=0;i<NTHREAD;i++)
    stack[i] = malloc(4096);
  mutex_init(&m);
  cond_init(&c);
  printf(1, "\nmutex and cv init complete\n");
  6e:	e8 7d 06 00 00       	call   6f0 <printf>
  73:	83 c4 10             	add    $0x10,%esp
  76:	8d 76 00             	lea    0x0(%esi),%esi
  79:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
  for(i=0;i<NTHREAD-1;i++){
    tid[i] = thread_create(thread, (void *)i, stack[i]);
  80:	83 ec 04             	sub    $0x4,%esp
  83:	ff 34 9d a0 0e 00 00 	pushl  0xea0(,%ebx,4)
  8a:	53                   	push   %ebx
  8b:	68 10 02 00 00       	push   $0x210
  90:	e8 5d 05 00 00       	call   5f2 <thread_create>
    printf(1, "tid[%d]: %d\n", i, tid[i]);
  95:	50                   	push   %eax
  96:	53                   	push   %ebx
  97:	68 94 0a 00 00       	push   $0xa94
  9c:	6a 01                	push   $0x1
    stack[i] = malloc(4096);
  mutex_init(&m);
  cond_init(&c);
  printf(1, "\nmutex and cv init complete\n");
  for(i=0;i<NTHREAD-1;i++){
    tid[i] = thread_create(thread, (void *)i, stack[i]);
  9e:	89 04 9d c0 0e 00 00 	mov    %eax,0xec0(,%ebx,4)
    printf(1, "tid[%d]: %d\n", i, tid[i]);
  a5:	e8 46 06 00 00       	call   6f0 <printf>
    if(tid[i] == -1){
  aa:	83 c4 20             	add    $0x20,%esp
  ad:	83 3c 9d c0 0e 00 00 	cmpl   $0xffffffff,0xec0(,%ebx,4)
  b4:	ff 
  b5:	0f 84 1c 01 00 00    	je     1d7 <main+0x1d7>
  for(i=0;i<NTHREAD;i++)
    stack[i] = malloc(4096);
  mutex_init(&m);
  cond_init(&c);
  printf(1, "\nmutex and cv init complete\n");
  for(i=0;i<NTHREAD-1;i++){
  bb:	83 c3 01             	add    $0x1,%ebx
  be:	83 fb 07             	cmp    $0x7,%ebx
  c1:	75 bd                	jne    80 <main+0x80>
    if(tid[i] == -1){
      printf(1, "thread create failed\n");
      exit();
    }
  }
  printf(1, "real process going cond_signal\n");
  c3:	83 ec 08             	sub    $0x8,%esp
  c6:	68 58 0b 00 00       	push   $0xb58
  cb:	6a 01                	push   $0x1
  cd:	e8 1e 06 00 00       	call   6f0 <printf>
  d2:	83 c4 10             	add    $0x10,%esp
  d5:	8d 76 00             	lea    0x0(%esi),%esi
  for(i=0;i<NTHREAD-1;i++){
    mutex_lock(&m);
  d8:	83 ec 0c             	sub    $0xc,%esp
  db:	68 c0 0f 00 00       	push   $0xfc0
  e0:	e8 35 05 00 00       	call   61a <mutex_lock>
    condition++;
    printf(1, "condition signal!!!!\n");
  e5:	58                   	pop    %eax
  e6:	5a                   	pop    %edx
  e7:	68 b7 0a 00 00       	push   $0xab7
  ec:	6a 01                	push   $0x1
    }
  }
  printf(1, "real process going cond_signal\n");
  for(i=0;i<NTHREAD-1;i++){
    mutex_lock(&m);
    condition++;
  ee:	83 05 80 0e 00 00 01 	addl   $0x1,0xe80
    printf(1, "condition signal!!!!\n");
  f5:	e8 f6 05 00 00       	call   6f0 <printf>
    cond_signal(&c);
  fa:	c7 04 24 20 0f 00 00 	movl   $0xf20,(%esp)
 101:	e8 34 05 00 00       	call   63a <cond_signal>
    mutex_unlock(&m);
 106:	c7 04 24 c0 0f 00 00 	movl   $0xfc0,(%esp)
 10d:	e8 10 05 00 00       	call   622 <mutex_unlock>
      printf(1, "thread create failed\n");
      exit();
    }
  }
  printf(1, "real process going cond_signal\n");
  for(i=0;i<NTHREAD-1;i++){
 112:	83 c4 10             	add    $0x10,%esp
 115:	83 eb 01             	sub    $0x1,%ebx
 118:	75 be                	jne    d8 <main+0xd8>
    printf(1, "condition signal!!!!\n");
    cond_signal(&c);
    mutex_unlock(&m);
  }

  printf(1, "real process going thread_join\n");
 11a:	83 ec 08             	sub    $0x8,%esp
 11d:	31 db                	xor    %ebx,%ebx
 11f:	68 78 0b 00 00       	push   $0xb78
 124:	6a 01                	push   $0x1
 126:	e8 c5 05 00 00       	call   6f0 <printf>
 12b:	83 c4 10             	add    $0x10,%esp
 12e:	66 90                	xchg   %ax,%ax
  for(i=0;i<NTHREAD-1;i++){
    if(thread_join(tid[i], &retval[i]) == -1){
 130:	8d 83 a0 0f 00 00    	lea    0xfa0(%ebx),%eax
 136:	83 ec 08             	sub    $0x8,%esp
 139:	50                   	push   %eax
 13a:	ff b3 c0 0e 00 00    	pushl  0xec0(%ebx)
 140:	e8 bd 04 00 00       	call   602 <thread_join>
 145:	83 c4 10             	add    $0x10,%esp
 148:	83 f8 ff             	cmp    $0xffffffff,%eax
 14b:	0f 84 99 00 00 00    	je     1ea <main+0x1ea>
      printf(1, "thread join failed\n");
      exit();
    }
    printf(1, "thread join!\n");
 151:	83 ec 08             	sub    $0x8,%esp
 154:	83 c3 04             	add    $0x4,%ebx
 157:	68 e1 0a 00 00       	push   $0xae1
 15c:	6a 01                	push   $0x1
 15e:	e8 8d 05 00 00       	call   6f0 <printf>
    cond_signal(&c);
    mutex_unlock(&m);
  }

  printf(1, "real process going thread_join\n");
  for(i=0;i<NTHREAD-1;i++){
 163:	83 c4 10             	add    $0x10,%esp
 166:	83 fb 1c             	cmp    $0x1c,%ebx
 169:	75 c5                	jne    130 <main+0x130>
      printf(1, "thread join failed\n");
      exit();
    }
    printf(1, "thread join!\n");
  }
  printf(1, "thread_join complete\n");
 16b:	51                   	push   %ecx
 16c:	51                   	push   %ecx

  for(i=0;i<NTHREAD-1;i++){
 16d:	31 db                	xor    %ebx,%ebx
      printf(1, "thread join failed\n");
      exit();
    }
    printf(1, "thread join!\n");
  }
  printf(1, "thread_join complete\n");
 16f:	68 ef 0a 00 00       	push   $0xaef
 174:	6a 01                	push   $0x1
 176:	e8 75 05 00 00       	call   6f0 <printf>
 17b:	83 c4 10             	add    $0x10,%esp
 17e:	66 90                	xchg   %ax,%ax

  for(i=0;i<NTHREAD-1;i++){
    printf(1, "buffer[%d] = %d\n",i, buffer[i]);
 180:	ff 34 9d e0 0e 00 00 	pushl  0xee0(,%ebx,4)
 187:	53                   	push   %ebx
 188:	68 05 0b 00 00       	push   $0xb05
 18d:	6a 01                	push   $0x1
 18f:	e8 5c 05 00 00       	call   6f0 <printf>
    if(buffer[i] != i){
 194:	83 c4 10             	add    $0x10,%esp
 197:	39 1c 9d e0 0e 00 00 	cmp    %ebx,0xee0(,%ebx,4)
 19e:	75 5d                	jne    1fd <main+0x1fd>
    }
    printf(1, "thread join!\n");
  }
  printf(1, "thread_join complete\n");

  for(i=0;i<NTHREAD-1;i++){
 1a0:	83 c3 01             	add    $0x1,%ebx
 1a3:	83 fb 07             	cmp    $0x7,%ebx
 1a6:	75 d8                	jne    180 <main+0x180>
 1a8:	90                   	nop
 1a9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      exit();
    }
  }

  for(i=0;i<NTHREAD;i++)
    free(stack[i]);
 1b0:	83 ec 0c             	sub    $0xc,%esp
 1b3:	ff 36                	pushl  (%esi)
 1b5:	83 c6 04             	add    $0x4,%esi
 1b8:	e8 d3 06 00 00       	call   890 <free>
      printf(1, "WRONG\n");
      exit();
    }
  }

  for(i=0;i<NTHREAD;i++)
 1bd:	83 c4 10             	add    $0x10,%esp
 1c0:	39 f7                	cmp    %esi,%edi
 1c2:	75 ec                	jne    1b0 <main+0x1b0>
    free(stack[i]);

  printf(1, "OK\n");
 1c4:	50                   	push   %eax
 1c5:	50                   	push   %eax
 1c6:	68 1d 0b 00 00       	push   $0xb1d
 1cb:	6a 01                	push   $0x1
 1cd:	e8 1e 05 00 00       	call   6f0 <printf>

  exit();
 1d2:	e8 5b 03 00 00       	call   532 <exit>
  printf(1, "\nmutex and cv init complete\n");
  for(i=0;i<NTHREAD-1;i++){
    tid[i] = thread_create(thread, (void *)i, stack[i]);
    printf(1, "tid[%d]: %d\n", i, tid[i]);
    if(tid[i] == -1){
      printf(1, "thread create failed\n");
 1d7:	51                   	push   %ecx
 1d8:	51                   	push   %ecx
 1d9:	68 a1 0a 00 00       	push   $0xaa1
 1de:	6a 01                	push   $0x1
 1e0:	e8 0b 05 00 00       	call   6f0 <printf>
      exit();
 1e5:	e8 48 03 00 00       	call   532 <exit>
  }

  printf(1, "real process going thread_join\n");
  for(i=0;i<NTHREAD-1;i++){
    if(thread_join(tid[i], &retval[i]) == -1){
      printf(1, "thread join failed\n");
 1ea:	53                   	push   %ebx
 1eb:	53                   	push   %ebx
 1ec:	68 cd 0a 00 00       	push   $0xacd
 1f1:	6a 01                	push   $0x1
 1f3:	e8 f8 04 00 00       	call   6f0 <printf>
      exit();
 1f8:	e8 35 03 00 00       	call   532 <exit>
  printf(1, "thread_join complete\n");

  for(i=0;i<NTHREAD-1;i++){
    printf(1, "buffer[%d] = %d\n",i, buffer[i]);
    if(buffer[i] != i){
      printf(1, "WRONG\n");
 1fd:	52                   	push   %edx
 1fe:	52                   	push   %edx
 1ff:	68 16 0b 00 00       	push   $0xb16
 204:	6a 01                	push   $0x1
 206:	e8 e5 04 00 00       	call   6f0 <printf>
      exit();
 20b:	e8 22 03 00 00       	call   532 <exit>

00000210 <thread>:
struct cond_t c;
int buffer[10];
int idx=0;
int condition = 0;

void *thread(void *arg){
 210:	55                   	push   %ebp
 211:	89 e5                	mov    %esp,%ebp
 213:	53                   	push   %ebx
 214:	83 ec 0c             	sub    $0xc,%esp
 217:	8b 5d 08             	mov    0x8(%ebp),%ebx
  printf(1, "19191919191919191919\n");
 21a:	68 10 0a 00 00       	push   $0xa10
 21f:	6a 01                	push   $0x1
 221:	e8 ca 04 00 00       	call   6f0 <printf>
  mutex_lock(&m);
 226:	c7 04 24 c0 0f 00 00 	movl   $0xfc0,(%esp)
 22d:	e8 e8 03 00 00       	call   61a <mutex_lock>
  printf(1, "************\n");
 232:	58                   	pop    %eax
 233:	5a                   	pop    %edx
 234:	68 26 0a 00 00       	push   $0xa26
 239:	6a 01                	push   $0x1
 23b:	e8 b0 04 00 00       	call   6f0 <printf>
  while(condition == 0){
 240:	8b 0d 80 0e 00 00    	mov    0xe80,%ecx
 246:	83 c4 10             	add    $0x10,%esp
 249:	85 c9                	test   %ecx,%ecx
 24b:	75 2f                	jne    27c <thread+0x6c>
 24d:	8d 76 00             	lea    0x0(%esi),%esi
    printf(1, "----------------cond_wait while loop--------------\n");
 250:	83 ec 08             	sub    $0x8,%esp
 253:	68 24 0b 00 00       	push   $0xb24
 258:	6a 01                	push   $0x1
 25a:	e8 91 04 00 00       	call   6f0 <printf>
    cond_wait(&c, &m);
 25f:	59                   	pop    %ecx
 260:	58                   	pop    %eax
 261:	68 c0 0f 00 00       	push   $0xfc0
 266:	68 20 0f 00 00       	push   $0xf20
 26b:	e8 c2 03 00 00       	call   632 <cond_wait>

void *thread(void *arg){
  printf(1, "19191919191919191919\n");
  mutex_lock(&m);
  printf(1, "************\n");
  while(condition == 0){
 270:	a1 80 0e 00 00       	mov    0xe80,%eax
 275:	83 c4 10             	add    $0x10,%esp
 278:	85 c0                	test   %eax,%eax
 27a:	74 d4                	je     250 <thread+0x40>
    printf(1, "----------------cond_wait while loop--------------\n");
    cond_wait(&c, &m);
  }
  printf(1, "cond_wait complete\n");
 27c:	83 ec 08             	sub    $0x8,%esp
 27f:	68 34 0a 00 00       	push   $0xa34
 284:	6a 01                	push   $0x1
 286:	e8 65 04 00 00       	call   6f0 <printf>


  condition--;
  buffer[idx++] = (int)arg;
 28b:	a1 84 0e 00 00       	mov    0xe84,%eax
  printf(1, "arg: %d\t buffer[idx]: %d\n", (int)arg, buffer[idx-1]);
 290:	53                   	push   %ebx
 291:	53                   	push   %ebx
 292:	68 48 0a 00 00       	push   $0xa48
 297:	6a 01                	push   $0x1
    cond_wait(&c, &m);
  }
  printf(1, "cond_wait complete\n");


  condition--;
 299:	83 2d 80 0e 00 00 01 	subl   $0x1,0xe80
  buffer[idx++] = (int)arg;
 2a0:	8d 50 01             	lea    0x1(%eax),%edx
 2a3:	89 1c 85 e0 0e 00 00 	mov    %ebx,0xee0(,%eax,4)
 2aa:	89 15 84 0e 00 00    	mov    %edx,0xe84
  printf(1, "arg: %d\t buffer[idx]: %d\n", (int)arg, buffer[idx-1]);
 2b0:	e8 3b 04 00 00       	call   6f0 <printf>
  mutex_unlock(&m);
 2b5:	83 c4 14             	add    $0x14,%esp
 2b8:	68 c0 0f 00 00       	push   $0xfc0
 2bd:	e8 60 03 00 00       	call   622 <mutex_unlock>
  printf(1, "thread_exit\n\n");
 2c2:	58                   	pop    %eax
 2c3:	5a                   	pop    %edx
 2c4:	68 62 0a 00 00       	push   $0xa62
 2c9:	6a 01                	push   $0x1
 2cb:	e8 20 04 00 00       	call   6f0 <printf>
  thread_exit(0);
 2d0:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 2d7:	e8 1e 03 00 00       	call   5fa <thread_exit>
}
 2dc:	31 c0                	xor    %eax,%eax
 2de:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 2e1:	c9                   	leave  
 2e2:	c3                   	ret    
 2e3:	66 90                	xchg   %ax,%ax
 2e5:	66 90                	xchg   %ax,%ax
 2e7:	66 90                	xchg   %ax,%ax
 2e9:	66 90                	xchg   %ax,%ax
 2eb:	66 90                	xchg   %ax,%ax
 2ed:	66 90                	xchg   %ax,%ax
 2ef:	90                   	nop

000002f0 <strcpy>:
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
 2f0:	55                   	push   %ebp
 2f1:	89 e5                	mov    %esp,%ebp
 2f3:	53                   	push   %ebx
 2f4:	8b 45 08             	mov    0x8(%ebp),%eax
 2f7:	8b 4d 0c             	mov    0xc(%ebp),%ecx
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 2fa:	89 c2                	mov    %eax,%edx
 2fc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 300:	83 c1 01             	add    $0x1,%ecx
 303:	0f b6 59 ff          	movzbl -0x1(%ecx),%ebx
 307:	83 c2 01             	add    $0x1,%edx
 30a:	84 db                	test   %bl,%bl
 30c:	88 5a ff             	mov    %bl,-0x1(%edx)
 30f:	75 ef                	jne    300 <strcpy+0x10>
    ;
  return os;
}
 311:	5b                   	pop    %ebx
 312:	5d                   	pop    %ebp
 313:	c3                   	ret    
 314:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 31a:	8d bf 00 00 00 00    	lea    0x0(%edi),%edi

00000320 <strcmp>:

int
strcmp(const char *p, const char *q)
{
 320:	55                   	push   %ebp
 321:	89 e5                	mov    %esp,%ebp
 323:	56                   	push   %esi
 324:	53                   	push   %ebx
 325:	8b 55 08             	mov    0x8(%ebp),%edx
 328:	8b 4d 0c             	mov    0xc(%ebp),%ecx
  while(*p && *p == *q)
 32b:	0f b6 02             	movzbl (%edx),%eax
 32e:	0f b6 19             	movzbl (%ecx),%ebx
 331:	84 c0                	test   %al,%al
 333:	75 1e                	jne    353 <strcmp+0x33>
 335:	eb 29                	jmp    360 <strcmp+0x40>
 337:	89 f6                	mov    %esi,%esi
 339:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
    p++, q++;
 340:	83 c2 01             	add    $0x1,%edx
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
 343:	0f b6 02             	movzbl (%edx),%eax
    p++, q++;
 346:	8d 71 01             	lea    0x1(%ecx),%esi
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
 349:	0f b6 59 01          	movzbl 0x1(%ecx),%ebx
 34d:	84 c0                	test   %al,%al
 34f:	74 0f                	je     360 <strcmp+0x40>
 351:	89 f1                	mov    %esi,%ecx
 353:	38 d8                	cmp    %bl,%al
 355:	74 e9                	je     340 <strcmp+0x20>
    p++, q++;
  return (uchar)*p - (uchar)*q;
 357:	29 d8                	sub    %ebx,%eax
}
 359:	5b                   	pop    %ebx
 35a:	5e                   	pop    %esi
 35b:	5d                   	pop    %ebp
 35c:	c3                   	ret    
 35d:	8d 76 00             	lea    0x0(%esi),%esi
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
 360:	31 c0                	xor    %eax,%eax
    p++, q++;
  return (uchar)*p - (uchar)*q;
 362:	29 d8                	sub    %ebx,%eax
}
 364:	5b                   	pop    %ebx
 365:	5e                   	pop    %esi
 366:	5d                   	pop    %ebp
 367:	c3                   	ret    
 368:	90                   	nop
 369:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

00000370 <strlen>:

uint
strlen(const char *s)
{
 370:	55                   	push   %ebp
 371:	89 e5                	mov    %esp,%ebp
 373:	8b 4d 08             	mov    0x8(%ebp),%ecx
  int n;

  for(n = 0; s[n]; n++)
 376:	80 39 00             	cmpb   $0x0,(%ecx)
 379:	74 12                	je     38d <strlen+0x1d>
 37b:	31 d2                	xor    %edx,%edx
 37d:	8d 76 00             	lea    0x0(%esi),%esi
 380:	83 c2 01             	add    $0x1,%edx
 383:	80 3c 11 00          	cmpb   $0x0,(%ecx,%edx,1)
 387:	89 d0                	mov    %edx,%eax
 389:	75 f5                	jne    380 <strlen+0x10>
    ;
  return n;
}
 38b:	5d                   	pop    %ebp
 38c:	c3                   	ret    
uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
 38d:	31 c0                	xor    %eax,%eax
    ;
  return n;
}
 38f:	5d                   	pop    %ebp
 390:	c3                   	ret    
 391:	eb 0d                	jmp    3a0 <memset>
 393:	90                   	nop
 394:	90                   	nop
 395:	90                   	nop
 396:	90                   	nop
 397:	90                   	nop
 398:	90                   	nop
 399:	90                   	nop
 39a:	90                   	nop
 39b:	90                   	nop
 39c:	90                   	nop
 39d:	90                   	nop
 39e:	90                   	nop
 39f:	90                   	nop

000003a0 <memset>:

void*
memset(void *dst, int c, uint n)
{
 3a0:	55                   	push   %ebp
 3a1:	89 e5                	mov    %esp,%ebp
 3a3:	57                   	push   %edi
 3a4:	8b 55 08             	mov    0x8(%ebp),%edx
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
 3a7:	8b 4d 10             	mov    0x10(%ebp),%ecx
 3aa:	8b 45 0c             	mov    0xc(%ebp),%eax
 3ad:	89 d7                	mov    %edx,%edi
 3af:	fc                   	cld    
 3b0:	f3 aa                	rep stos %al,%es:(%edi)
  stosb(dst, c, n);
  return dst;
}
 3b2:	89 d0                	mov    %edx,%eax
 3b4:	5f                   	pop    %edi
 3b5:	5d                   	pop    %ebp
 3b6:	c3                   	ret    
 3b7:	89 f6                	mov    %esi,%esi
 3b9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

000003c0 <strchr>:

char*
strchr(const char *s, char c)
{
 3c0:	55                   	push   %ebp
 3c1:	89 e5                	mov    %esp,%ebp
 3c3:	53                   	push   %ebx
 3c4:	8b 45 08             	mov    0x8(%ebp),%eax
 3c7:	8b 5d 0c             	mov    0xc(%ebp),%ebx
  for(; *s; s++)
 3ca:	0f b6 10             	movzbl (%eax),%edx
 3cd:	84 d2                	test   %dl,%dl
 3cf:	74 1d                	je     3ee <strchr+0x2e>
    if(*s == c)
 3d1:	38 d3                	cmp    %dl,%bl
 3d3:	89 d9                	mov    %ebx,%ecx
 3d5:	75 0d                	jne    3e4 <strchr+0x24>
 3d7:	eb 17                	jmp    3f0 <strchr+0x30>
 3d9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 3e0:	38 ca                	cmp    %cl,%dl
 3e2:	74 0c                	je     3f0 <strchr+0x30>
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
 3e4:	83 c0 01             	add    $0x1,%eax
 3e7:	0f b6 10             	movzbl (%eax),%edx
 3ea:	84 d2                	test   %dl,%dl
 3ec:	75 f2                	jne    3e0 <strchr+0x20>
    if(*s == c)
      return (char*)s;
  return 0;
 3ee:	31 c0                	xor    %eax,%eax
}
 3f0:	5b                   	pop    %ebx
 3f1:	5d                   	pop    %ebp
 3f2:	c3                   	ret    
 3f3:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 3f9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

00000400 <gets>:

char*
gets(char *buf, int max)
{
 400:	55                   	push   %ebp
 401:	89 e5                	mov    %esp,%ebp
 403:	57                   	push   %edi
 404:	56                   	push   %esi
 405:	53                   	push   %ebx
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 406:	31 f6                	xor    %esi,%esi
    cc = read(0, &c, 1);
 408:	8d 7d e7             	lea    -0x19(%ebp),%edi
  return 0;
}

char*
gets(char *buf, int max)
{
 40b:	83 ec 1c             	sub    $0x1c,%esp
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 40e:	eb 29                	jmp    439 <gets+0x39>
    cc = read(0, &c, 1);
 410:	83 ec 04             	sub    $0x4,%esp
 413:	6a 01                	push   $0x1
 415:	57                   	push   %edi
 416:	6a 00                	push   $0x0
 418:	e8 2d 01 00 00       	call   54a <read>
    if(cc < 1)
 41d:	83 c4 10             	add    $0x10,%esp
 420:	85 c0                	test   %eax,%eax
 422:	7e 1d                	jle    441 <gets+0x41>
      break;
    buf[i++] = c;
 424:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 428:	8b 55 08             	mov    0x8(%ebp),%edx
 42b:	89 de                	mov    %ebx,%esi
    if(c == '\n' || c == '\r')
 42d:	3c 0a                	cmp    $0xa,%al

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
 42f:	88 44 1a ff          	mov    %al,-0x1(%edx,%ebx,1)
    if(c == '\n' || c == '\r')
 433:	74 1b                	je     450 <gets+0x50>
 435:	3c 0d                	cmp    $0xd,%al
 437:	74 17                	je     450 <gets+0x50>
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 439:	8d 5e 01             	lea    0x1(%esi),%ebx
 43c:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
 43f:	7c cf                	jl     410 <gets+0x10>
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
 441:	8b 45 08             	mov    0x8(%ebp),%eax
 444:	c6 04 30 00          	movb   $0x0,(%eax,%esi,1)
  return buf;
}
 448:	8d 65 f4             	lea    -0xc(%ebp),%esp
 44b:	5b                   	pop    %ebx
 44c:	5e                   	pop    %esi
 44d:	5f                   	pop    %edi
 44e:	5d                   	pop    %ebp
 44f:	c3                   	ret    
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
 450:	8b 45 08             	mov    0x8(%ebp),%eax
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 453:	89 de                	mov    %ebx,%esi
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
 455:	c6 04 30 00          	movb   $0x0,(%eax,%esi,1)
  return buf;
}
 459:	8d 65 f4             	lea    -0xc(%ebp),%esp
 45c:	5b                   	pop    %ebx
 45d:	5e                   	pop    %esi
 45e:	5f                   	pop    %edi
 45f:	5d                   	pop    %ebp
 460:	c3                   	ret    
 461:	eb 0d                	jmp    470 <stat>
 463:	90                   	nop
 464:	90                   	nop
 465:	90                   	nop
 466:	90                   	nop
 467:	90                   	nop
 468:	90                   	nop
 469:	90                   	nop
 46a:	90                   	nop
 46b:	90                   	nop
 46c:	90                   	nop
 46d:	90                   	nop
 46e:	90                   	nop
 46f:	90                   	nop

00000470 <stat>:

int
stat(const char *n, struct stat *st)
{
 470:	55                   	push   %ebp
 471:	89 e5                	mov    %esp,%ebp
 473:	56                   	push   %esi
 474:	53                   	push   %ebx
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 475:	83 ec 08             	sub    $0x8,%esp
 478:	6a 00                	push   $0x0
 47a:	ff 75 08             	pushl  0x8(%ebp)
 47d:	e8 f0 00 00 00       	call   572 <open>
  if(fd < 0)
 482:	83 c4 10             	add    $0x10,%esp
 485:	85 c0                	test   %eax,%eax
 487:	78 27                	js     4b0 <stat+0x40>
    return -1;
  r = fstat(fd, st);
 489:	83 ec 08             	sub    $0x8,%esp
 48c:	ff 75 0c             	pushl  0xc(%ebp)
 48f:	89 c3                	mov    %eax,%ebx
 491:	50                   	push   %eax
 492:	e8 f3 00 00 00       	call   58a <fstat>
 497:	89 c6                	mov    %eax,%esi
  close(fd);
 499:	89 1c 24             	mov    %ebx,(%esp)
 49c:	e8 b9 00 00 00       	call   55a <close>
  return r;
 4a1:	83 c4 10             	add    $0x10,%esp
 4a4:	89 f0                	mov    %esi,%eax
}
 4a6:	8d 65 f8             	lea    -0x8(%ebp),%esp
 4a9:	5b                   	pop    %ebx
 4aa:	5e                   	pop    %esi
 4ab:	5d                   	pop    %ebp
 4ac:	c3                   	ret    
 4ad:	8d 76 00             	lea    0x0(%esi),%esi
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
 4b0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
 4b5:	eb ef                	jmp    4a6 <stat+0x36>
 4b7:	89 f6                	mov    %esi,%esi
 4b9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

000004c0 <atoi>:
  return r;
}

int
atoi(const char *s)
{
 4c0:	55                   	push   %ebp
 4c1:	89 e5                	mov    %esp,%ebp
 4c3:	53                   	push   %ebx
 4c4:	8b 4d 08             	mov    0x8(%ebp),%ecx
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 4c7:	0f be 11             	movsbl (%ecx),%edx
 4ca:	8d 42 d0             	lea    -0x30(%edx),%eax
 4cd:	3c 09                	cmp    $0x9,%al
 4cf:	b8 00 00 00 00       	mov    $0x0,%eax
 4d4:	77 1f                	ja     4f5 <atoi+0x35>
 4d6:	8d 76 00             	lea    0x0(%esi),%esi
 4d9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
    n = n*10 + *s++ - '0';
 4e0:	8d 04 80             	lea    (%eax,%eax,4),%eax
 4e3:	83 c1 01             	add    $0x1,%ecx
 4e6:	8d 44 42 d0          	lea    -0x30(%edx,%eax,2),%eax
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 4ea:	0f be 11             	movsbl (%ecx),%edx
 4ed:	8d 5a d0             	lea    -0x30(%edx),%ebx
 4f0:	80 fb 09             	cmp    $0x9,%bl
 4f3:	76 eb                	jbe    4e0 <atoi+0x20>
    n = n*10 + *s++ - '0';
  return n;
}
 4f5:	5b                   	pop    %ebx
 4f6:	5d                   	pop    %ebp
 4f7:	c3                   	ret    
 4f8:	90                   	nop
 4f9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

00000500 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 500:	55                   	push   %ebp
 501:	89 e5                	mov    %esp,%ebp
 503:	56                   	push   %esi
 504:	53                   	push   %ebx
 505:	8b 5d 10             	mov    0x10(%ebp),%ebx
 508:	8b 45 08             	mov    0x8(%ebp),%eax
 50b:	8b 75 0c             	mov    0xc(%ebp),%esi
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 50e:	85 db                	test   %ebx,%ebx
 510:	7e 14                	jle    526 <memmove+0x26>
 512:	31 d2                	xor    %edx,%edx
 514:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
    *dst++ = *src++;
 518:	0f b6 0c 16          	movzbl (%esi,%edx,1),%ecx
 51c:	88 0c 10             	mov    %cl,(%eax,%edx,1)
 51f:	83 c2 01             	add    $0x1,%edx
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 522:	39 da                	cmp    %ebx,%edx
 524:	75 f2                	jne    518 <memmove+0x18>
    *dst++ = *src++;
  return vdst;
}
 526:	5b                   	pop    %ebx
 527:	5e                   	pop    %esi
 528:	5d                   	pop    %ebp
 529:	c3                   	ret    

0000052a <fork>:
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
 52a:	b8 01 00 00 00       	mov    $0x1,%eax
 52f:	cd 40                	int    $0x40
 531:	c3                   	ret    

00000532 <exit>:
SYSCALL(exit)
 532:	b8 02 00 00 00       	mov    $0x2,%eax
 537:	cd 40                	int    $0x40
 539:	c3                   	ret    

0000053a <wait>:
SYSCALL(wait)
 53a:	b8 03 00 00 00       	mov    $0x3,%eax
 53f:	cd 40                	int    $0x40
 541:	c3                   	ret    

00000542 <pipe>:
SYSCALL(pipe)
 542:	b8 04 00 00 00       	mov    $0x4,%eax
 547:	cd 40                	int    $0x40
 549:	c3                   	ret    

0000054a <read>:
SYSCALL(read)
 54a:	b8 05 00 00 00       	mov    $0x5,%eax
 54f:	cd 40                	int    $0x40
 551:	c3                   	ret    

00000552 <write>:
SYSCALL(write)
 552:	b8 10 00 00 00       	mov    $0x10,%eax
 557:	cd 40                	int    $0x40
 559:	c3                   	ret    

0000055a <close>:
SYSCALL(close)
 55a:	b8 15 00 00 00       	mov    $0x15,%eax
 55f:	cd 40                	int    $0x40
 561:	c3                   	ret    

00000562 <kill>:
SYSCALL(kill)
 562:	b8 06 00 00 00       	mov    $0x6,%eax
 567:	cd 40                	int    $0x40
 569:	c3                   	ret    

0000056a <exec>:
SYSCALL(exec)
 56a:	b8 07 00 00 00       	mov    $0x7,%eax
 56f:	cd 40                	int    $0x40
 571:	c3                   	ret    

00000572 <open>:
SYSCALL(open)
 572:	b8 0f 00 00 00       	mov    $0xf,%eax
 577:	cd 40                	int    $0x40
 579:	c3                   	ret    

0000057a <mknod>:
SYSCALL(mknod)
 57a:	b8 11 00 00 00       	mov    $0x11,%eax
 57f:	cd 40                	int    $0x40
 581:	c3                   	ret    

00000582 <unlink>:
SYSCALL(unlink)
 582:	b8 12 00 00 00       	mov    $0x12,%eax
 587:	cd 40                	int    $0x40
 589:	c3                   	ret    

0000058a <fstat>:
SYSCALL(fstat)
 58a:	b8 08 00 00 00       	mov    $0x8,%eax
 58f:	cd 40                	int    $0x40
 591:	c3                   	ret    

00000592 <link>:
SYSCALL(link)
 592:	b8 13 00 00 00       	mov    $0x13,%eax
 597:	cd 40                	int    $0x40
 599:	c3                   	ret    

0000059a <mkdir>:
SYSCALL(mkdir)
 59a:	b8 14 00 00 00       	mov    $0x14,%eax
 59f:	cd 40                	int    $0x40
 5a1:	c3                   	ret    

000005a2 <chdir>:
SYSCALL(chdir)
 5a2:	b8 09 00 00 00       	mov    $0x9,%eax
 5a7:	cd 40                	int    $0x40
 5a9:	c3                   	ret    

000005aa <dup>:
SYSCALL(dup)
 5aa:	b8 0a 00 00 00       	mov    $0xa,%eax
 5af:	cd 40                	int    $0x40
 5b1:	c3                   	ret    

000005b2 <getpid>:
SYSCALL(getpid)
 5b2:	b8 0b 00 00 00       	mov    $0xb,%eax
 5b7:	cd 40                	int    $0x40
 5b9:	c3                   	ret    

000005ba <sbrk>:
SYSCALL(sbrk)
 5ba:	b8 0c 00 00 00       	mov    $0xc,%eax
 5bf:	cd 40                	int    $0x40
 5c1:	c3                   	ret    

000005c2 <sleep>:
SYSCALL(sleep)
 5c2:	b8 0d 00 00 00       	mov    $0xd,%eax
 5c7:	cd 40                	int    $0x40
 5c9:	c3                   	ret    

000005ca <uptime>:
SYSCALL(uptime)
 5ca:	b8 0e 00 00 00       	mov    $0xe,%eax
 5cf:	cd 40                	int    $0x40
 5d1:	c3                   	ret    

000005d2 <halt>:
SYSCALL(halt)
 5d2:	b8 16 00 00 00       	mov    $0x16,%eax
 5d7:	cd 40                	int    $0x40
 5d9:	c3                   	ret    

000005da <getnice>:
SYSCALL(getnice)
 5da:	b8 17 00 00 00       	mov    $0x17,%eax
 5df:	cd 40                	int    $0x40
 5e1:	c3                   	ret    

000005e2 <setnice>:
SYSCALL(setnice)
 5e2:	b8 18 00 00 00       	mov    $0x18,%eax
 5e7:	cd 40                	int    $0x40
 5e9:	c3                   	ret    

000005ea <ps>:
SYSCALL(ps)
 5ea:	b8 19 00 00 00       	mov    $0x19,%eax
 5ef:	cd 40                	int    $0x40
 5f1:	c3                   	ret    

000005f2 <thread_create>:
SYSCALL(thread_create)
 5f2:	b8 1a 00 00 00       	mov    $0x1a,%eax
 5f7:	cd 40                	int    $0x40
 5f9:	c3                   	ret    

000005fa <thread_exit>:
SYSCALL(thread_exit)
 5fa:	b8 1b 00 00 00       	mov    $0x1b,%eax
 5ff:	cd 40                	int    $0x40
 601:	c3                   	ret    

00000602 <thread_join>:
SYSCALL(thread_join)
 602:	b8 1c 00 00 00       	mov    $0x1c,%eax
 607:	cd 40                	int    $0x40
 609:	c3                   	ret    

0000060a <gettid>:
SYSCALL(gettid)
 60a:	b8 1d 00 00 00       	mov    $0x1d,%eax
 60f:	cd 40                	int    $0x40
 611:	c3                   	ret    

00000612 <mutex_init>:
SYSCALL(mutex_init)
 612:	b8 1e 00 00 00       	mov    $0x1e,%eax
 617:	cd 40                	int    $0x40
 619:	c3                   	ret    

0000061a <mutex_lock>:
SYSCALL(mutex_lock)
 61a:	b8 1f 00 00 00       	mov    $0x1f,%eax
 61f:	cd 40                	int    $0x40
 621:	c3                   	ret    

00000622 <mutex_unlock>:
SYSCALL(mutex_unlock)
 622:	b8 20 00 00 00       	mov    $0x20,%eax
 627:	cd 40                	int    $0x40
 629:	c3                   	ret    

0000062a <cond_init>:
SYSCALL(cond_init)
 62a:	b8 21 00 00 00       	mov    $0x21,%eax
 62f:	cd 40                	int    $0x40
 631:	c3                   	ret    

00000632 <cond_wait>:
SYSCALL(cond_wait)
 632:	b8 22 00 00 00       	mov    $0x22,%eax
 637:	cd 40                	int    $0x40
 639:	c3                   	ret    

0000063a <cond_signal>:
SYSCALL(cond_signal)
 63a:	b8 23 00 00 00       	mov    $0x23,%eax
 63f:	cd 40                	int    $0x40
 641:	c3                   	ret    
 642:	66 90                	xchg   %ax,%ax
 644:	66 90                	xchg   %ax,%ax
 646:	66 90                	xchg   %ax,%ax
 648:	66 90                	xchg   %ax,%ax
 64a:	66 90                	xchg   %ax,%ax
 64c:	66 90                	xchg   %ax,%ax
 64e:	66 90                	xchg   %ax,%ax

00000650 <printint>:
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
 650:	55                   	push   %ebp
 651:	89 e5                	mov    %esp,%ebp
 653:	57                   	push   %edi
 654:	56                   	push   %esi
 655:	53                   	push   %ebx
 656:	89 c6                	mov    %eax,%esi
 658:	83 ec 3c             	sub    $0x3c,%esp
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 65b:	8b 5d 08             	mov    0x8(%ebp),%ebx
 65e:	85 db                	test   %ebx,%ebx
 660:	74 7e                	je     6e0 <printint+0x90>
 662:	89 d0                	mov    %edx,%eax
 664:	c1 e8 1f             	shr    $0x1f,%eax
 667:	84 c0                	test   %al,%al
 669:	74 75                	je     6e0 <printint+0x90>
    neg = 1;
    x = -xx;
 66b:	89 d0                	mov    %edx,%eax
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
 66d:	c7 45 c4 01 00 00 00 	movl   $0x1,-0x3c(%ebp)
    x = -xx;
 674:	f7 d8                	neg    %eax
 676:	89 75 c0             	mov    %esi,-0x40(%ebp)
  } else {
    x = xx;
  }

  i = 0;
 679:	31 ff                	xor    %edi,%edi
 67b:	8d 5d d7             	lea    -0x29(%ebp),%ebx
 67e:	89 ce                	mov    %ecx,%esi
 680:	eb 08                	jmp    68a <printint+0x3a>
 682:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  do{
    buf[i++] = digits[x % base];
 688:	89 cf                	mov    %ecx,%edi
 68a:	31 d2                	xor    %edx,%edx
 68c:	8d 4f 01             	lea    0x1(%edi),%ecx
 68f:	f7 f6                	div    %esi
 691:	0f b6 92 a0 0b 00 00 	movzbl 0xba0(%edx),%edx
  }while((x /= base) != 0);
 698:	85 c0                	test   %eax,%eax
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
 69a:	88 14 0b             	mov    %dl,(%ebx,%ecx,1)
  }while((x /= base) != 0);
 69d:	75 e9                	jne    688 <printint+0x38>
  if(neg)
 69f:	8b 45 c4             	mov    -0x3c(%ebp),%eax
 6a2:	8b 75 c0             	mov    -0x40(%ebp),%esi
 6a5:	85 c0                	test   %eax,%eax
 6a7:	74 08                	je     6b1 <printint+0x61>
    buf[i++] = '-';
 6a9:	c6 44 0d d8 2d       	movb   $0x2d,-0x28(%ebp,%ecx,1)
 6ae:	8d 4f 02             	lea    0x2(%edi),%ecx
 6b1:	8d 7c 0d d7          	lea    -0x29(%ebp,%ecx,1),%edi
 6b5:	8d 76 00             	lea    0x0(%esi),%esi
 6b8:	0f b6 07             	movzbl (%edi),%eax
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 6bb:	83 ec 04             	sub    $0x4,%esp
 6be:	83 ef 01             	sub    $0x1,%edi
 6c1:	6a 01                	push   $0x1
 6c3:	53                   	push   %ebx
 6c4:	56                   	push   %esi
 6c5:	88 45 d7             	mov    %al,-0x29(%ebp)
 6c8:	e8 85 fe ff ff       	call   552 <write>
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
 6cd:	83 c4 10             	add    $0x10,%esp
 6d0:	39 df                	cmp    %ebx,%edi
 6d2:	75 e4                	jne    6b8 <printint+0x68>
    putc(fd, buf[i]);
}
 6d4:	8d 65 f4             	lea    -0xc(%ebp),%esp
 6d7:	5b                   	pop    %ebx
 6d8:	5e                   	pop    %esi
 6d9:	5f                   	pop    %edi
 6da:	5d                   	pop    %ebp
 6db:	c3                   	ret    
 6dc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 6e0:	89 d0                	mov    %edx,%eax
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
 6e2:	c7 45 c4 00 00 00 00 	movl   $0x0,-0x3c(%ebp)
 6e9:	eb 8b                	jmp    676 <printint+0x26>
 6eb:	90                   	nop
 6ec:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

000006f0 <printf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 6f0:	55                   	push   %ebp
 6f1:	89 e5                	mov    %esp,%ebp
 6f3:	57                   	push   %edi
 6f4:	56                   	push   %esi
 6f5:	53                   	push   %ebx
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 6f6:	8d 45 10             	lea    0x10(%ebp),%eax
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 6f9:	83 ec 2c             	sub    $0x2c,%esp
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 6fc:	8b 75 0c             	mov    0xc(%ebp),%esi
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 6ff:	8b 7d 08             	mov    0x8(%ebp),%edi
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 702:	89 45 d0             	mov    %eax,-0x30(%ebp)
 705:	0f b6 1e             	movzbl (%esi),%ebx
 708:	83 c6 01             	add    $0x1,%esi
 70b:	84 db                	test   %bl,%bl
 70d:	0f 84 b0 00 00 00    	je     7c3 <printf+0xd3>
 713:	31 d2                	xor    %edx,%edx
 715:	eb 39                	jmp    750 <printf+0x60>
 717:	89 f6                	mov    %esi,%esi
 719:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
 720:	83 f8 25             	cmp    $0x25,%eax
 723:	89 55 d4             	mov    %edx,-0x2c(%ebp)
        state = '%';
 726:	ba 25 00 00 00       	mov    $0x25,%edx
  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
 72b:	74 18                	je     745 <printf+0x55>
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 72d:	8d 45 e2             	lea    -0x1e(%ebp),%eax
 730:	83 ec 04             	sub    $0x4,%esp
 733:	88 5d e2             	mov    %bl,-0x1e(%ebp)
 736:	6a 01                	push   $0x1
 738:	50                   	push   %eax
 739:	57                   	push   %edi
 73a:	e8 13 fe ff ff       	call   552 <write>
 73f:	8b 55 d4             	mov    -0x2c(%ebp),%edx
 742:	83 c4 10             	add    $0x10,%esp
 745:	83 c6 01             	add    $0x1,%esi
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 748:	0f b6 5e ff          	movzbl -0x1(%esi),%ebx
 74c:	84 db                	test   %bl,%bl
 74e:	74 73                	je     7c3 <printf+0xd3>
    c = fmt[i] & 0xff;
    if(state == 0){
 750:	85 d2                	test   %edx,%edx
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
 752:	0f be cb             	movsbl %bl,%ecx
 755:	0f b6 c3             	movzbl %bl,%eax
    if(state == 0){
 758:	74 c6                	je     720 <printf+0x30>
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 75a:	83 fa 25             	cmp    $0x25,%edx
 75d:	75 e6                	jne    745 <printf+0x55>
      if(c == 'd'){
 75f:	83 f8 64             	cmp    $0x64,%eax
 762:	0f 84 f8 00 00 00    	je     860 <printf+0x170>
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
 768:	81 e1 f7 00 00 00    	and    $0xf7,%ecx
 76e:	83 f9 70             	cmp    $0x70,%ecx
 771:	74 5d                	je     7d0 <printf+0xe0>
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
 773:	83 f8 73             	cmp    $0x73,%eax
 776:	0f 84 84 00 00 00    	je     800 <printf+0x110>
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
 77c:	83 f8 63             	cmp    $0x63,%eax
 77f:	0f 84 ea 00 00 00    	je     86f <printf+0x17f>
        putc(fd, *ap);
        ap++;
      } else if(c == '%'){
 785:	83 f8 25             	cmp    $0x25,%eax
 788:	0f 84 c2 00 00 00    	je     850 <printf+0x160>
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 78e:	8d 45 e7             	lea    -0x19(%ebp),%eax
 791:	83 ec 04             	sub    $0x4,%esp
 794:	c6 45 e7 25          	movb   $0x25,-0x19(%ebp)
 798:	6a 01                	push   $0x1
 79a:	50                   	push   %eax
 79b:	57                   	push   %edi
 79c:	e8 b1 fd ff ff       	call   552 <write>
 7a1:	83 c4 0c             	add    $0xc,%esp
 7a4:	8d 45 e6             	lea    -0x1a(%ebp),%eax
 7a7:	88 5d e6             	mov    %bl,-0x1a(%ebp)
 7aa:	6a 01                	push   $0x1
 7ac:	50                   	push   %eax
 7ad:	57                   	push   %edi
 7ae:	83 c6 01             	add    $0x1,%esi
 7b1:	e8 9c fd ff ff       	call   552 <write>
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 7b6:	0f b6 5e ff          	movzbl -0x1(%esi),%ebx
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 7ba:	83 c4 10             	add    $0x10,%esp
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 7bd:	31 d2                	xor    %edx,%edx
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 7bf:	84 db                	test   %bl,%bl
 7c1:	75 8d                	jne    750 <printf+0x60>
        putc(fd, c);
      }
      state = 0;
    }
  }
}
 7c3:	8d 65 f4             	lea    -0xc(%ebp),%esp
 7c6:	5b                   	pop    %ebx
 7c7:	5e                   	pop    %esi
 7c8:	5f                   	pop    %edi
 7c9:	5d                   	pop    %ebp
 7ca:	c3                   	ret    
 7cb:	90                   	nop
 7cc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
    } else if(state == '%'){
      if(c == 'd'){
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        printint(fd, *ap, 16, 0);
 7d0:	83 ec 0c             	sub    $0xc,%esp
 7d3:	b9 10 00 00 00       	mov    $0x10,%ecx
 7d8:	6a 00                	push   $0x0
 7da:	8b 5d d0             	mov    -0x30(%ebp),%ebx
 7dd:	89 f8                	mov    %edi,%eax
 7df:	8b 13                	mov    (%ebx),%edx
 7e1:	e8 6a fe ff ff       	call   650 <printint>
        ap++;
 7e6:	89 d8                	mov    %ebx,%eax
 7e8:	83 c4 10             	add    $0x10,%esp
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 7eb:	31 d2                	xor    %edx,%edx
      if(c == 'd'){
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        printint(fd, *ap, 16, 0);
        ap++;
 7ed:	83 c0 04             	add    $0x4,%eax
 7f0:	89 45 d0             	mov    %eax,-0x30(%ebp)
 7f3:	e9 4d ff ff ff       	jmp    745 <printf+0x55>
 7f8:	90                   	nop
 7f9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      } else if(c == 's'){
        s = (char*)*ap;
 800:	8b 45 d0             	mov    -0x30(%ebp),%eax
 803:	8b 18                	mov    (%eax),%ebx
        ap++;
 805:	83 c0 04             	add    $0x4,%eax
 808:	89 45 d0             	mov    %eax,-0x30(%ebp)
        if(s == 0)
          s = "(null)";
 80b:	b8 98 0b 00 00       	mov    $0xb98,%eax
 810:	85 db                	test   %ebx,%ebx
 812:	0f 44 d8             	cmove  %eax,%ebx
        while(*s != 0){
 815:	0f b6 03             	movzbl (%ebx),%eax
 818:	84 c0                	test   %al,%al
 81a:	74 23                	je     83f <printf+0x14f>
 81c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 820:	88 45 e3             	mov    %al,-0x1d(%ebp)
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 823:	8d 45 e3             	lea    -0x1d(%ebp),%eax
 826:	83 ec 04             	sub    $0x4,%esp
 829:	6a 01                	push   $0x1
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
 82b:	83 c3 01             	add    $0x1,%ebx
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 82e:	50                   	push   %eax
 82f:	57                   	push   %edi
 830:	e8 1d fd ff ff       	call   552 <write>
      } else if(c == 's'){
        s = (char*)*ap;
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
 835:	0f b6 03             	movzbl (%ebx),%eax
 838:	83 c4 10             	add    $0x10,%esp
 83b:	84 c0                	test   %al,%al
 83d:	75 e1                	jne    820 <printf+0x130>
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
 83f:	31 d2                	xor    %edx,%edx
 841:	e9 ff fe ff ff       	jmp    745 <printf+0x55>
 846:	8d 76 00             	lea    0x0(%esi),%esi
 849:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 850:	83 ec 04             	sub    $0x4,%esp
 853:	88 5d e5             	mov    %bl,-0x1b(%ebp)
 856:	8d 45 e5             	lea    -0x1b(%ebp),%eax
 859:	6a 01                	push   $0x1
 85b:	e9 4c ff ff ff       	jmp    7ac <printf+0xbc>
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
      if(c == 'd'){
        printint(fd, *ap, 10, 1);
 860:	83 ec 0c             	sub    $0xc,%esp
 863:	b9 0a 00 00 00       	mov    $0xa,%ecx
 868:	6a 01                	push   $0x1
 86a:	e9 6b ff ff ff       	jmp    7da <printf+0xea>
 86f:	8b 5d d0             	mov    -0x30(%ebp),%ebx
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
 872:	83 ec 04             	sub    $0x4,%esp
 875:	8b 03                	mov    (%ebx),%eax
 877:	6a 01                	push   $0x1
 879:	88 45 e4             	mov    %al,-0x1c(%ebp)
 87c:	8d 45 e4             	lea    -0x1c(%ebp),%eax
 87f:	50                   	push   %eax
 880:	57                   	push   %edi
 881:	e8 cc fc ff ff       	call   552 <write>
 886:	e9 5b ff ff ff       	jmp    7e6 <printf+0xf6>
 88b:	66 90                	xchg   %ax,%ax
 88d:	66 90                	xchg   %ax,%ax
 88f:	90                   	nop

00000890 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 890:	55                   	push   %ebp
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 891:	a1 88 0e 00 00       	mov    0xe88,%eax
static Header base;
static Header *freep;

void
free(void *ap)
{
 896:	89 e5                	mov    %esp,%ebp
 898:	57                   	push   %edi
 899:	56                   	push   %esi
 89a:	53                   	push   %ebx
 89b:	8b 5d 08             	mov    0x8(%ebp),%ebx
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 89e:	8b 10                	mov    (%eax),%edx
void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
 8a0:	8d 4b f8             	lea    -0x8(%ebx),%ecx
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8a3:	39 c8                	cmp    %ecx,%eax
 8a5:	73 19                	jae    8c0 <free+0x30>
 8a7:	89 f6                	mov    %esi,%esi
 8a9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
 8b0:	39 d1                	cmp    %edx,%ecx
 8b2:	72 1c                	jb     8d0 <free+0x40>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8b4:	39 d0                	cmp    %edx,%eax
 8b6:	73 18                	jae    8d0 <free+0x40>
static Header base;
static Header *freep;

void
free(void *ap)
{
 8b8:	89 d0                	mov    %edx,%eax
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8ba:	39 c8                	cmp    %ecx,%eax
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8bc:	8b 10                	mov    (%eax),%edx
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 8be:	72 f0                	jb     8b0 <free+0x20>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 8c0:	39 d0                	cmp    %edx,%eax
 8c2:	72 f4                	jb     8b8 <free+0x28>
 8c4:	39 d1                	cmp    %edx,%ecx
 8c6:	73 f0                	jae    8b8 <free+0x28>
 8c8:	90                   	nop
 8c9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      break;
  if(bp + bp->s.size == p->s.ptr){
 8d0:	8b 73 fc             	mov    -0x4(%ebx),%esi
 8d3:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 8d6:	39 d7                	cmp    %edx,%edi
 8d8:	74 19                	je     8f3 <free+0x63>
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
 8da:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 8dd:	8b 50 04             	mov    0x4(%eax),%edx
 8e0:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 8e3:	39 f1                	cmp    %esi,%ecx
 8e5:	74 23                	je     90a <free+0x7a>
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
 8e7:	89 08                	mov    %ecx,(%eax)
  freep = p;
 8e9:	a3 88 0e 00 00       	mov    %eax,0xe88
}
 8ee:	5b                   	pop    %ebx
 8ef:	5e                   	pop    %esi
 8f0:	5f                   	pop    %edi
 8f1:	5d                   	pop    %ebp
 8f2:	c3                   	ret    
  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 8f3:	03 72 04             	add    0x4(%edx),%esi
 8f6:	89 73 fc             	mov    %esi,-0x4(%ebx)
    bp->s.ptr = p->s.ptr->s.ptr;
 8f9:	8b 10                	mov    (%eax),%edx
 8fb:	8b 12                	mov    (%edx),%edx
 8fd:	89 53 f8             	mov    %edx,-0x8(%ebx)
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
 900:	8b 50 04             	mov    0x4(%eax),%edx
 903:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 906:	39 f1                	cmp    %esi,%ecx
 908:	75 dd                	jne    8e7 <free+0x57>
    p->s.size += bp->s.size;
 90a:	03 53 fc             	add    -0x4(%ebx),%edx
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
 90d:	a3 88 0e 00 00       	mov    %eax,0xe88
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 912:	89 50 04             	mov    %edx,0x4(%eax)
    p->s.ptr = bp->s.ptr;
 915:	8b 53 f8             	mov    -0x8(%ebx),%edx
 918:	89 10                	mov    %edx,(%eax)
  } else
    p->s.ptr = bp;
  freep = p;
}
 91a:	5b                   	pop    %ebx
 91b:	5e                   	pop    %esi
 91c:	5f                   	pop    %edi
 91d:	5d                   	pop    %ebp
 91e:	c3                   	ret    
 91f:	90                   	nop

00000920 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 920:	55                   	push   %ebp
 921:	89 e5                	mov    %esp,%ebp
 923:	57                   	push   %edi
 924:	56                   	push   %esi
 925:	53                   	push   %ebx
 926:	83 ec 0c             	sub    $0xc,%esp
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 929:	8b 45 08             	mov    0x8(%ebp),%eax
  if((prevp = freep) == 0){
 92c:	8b 15 88 0e 00 00    	mov    0xe88,%edx
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 932:	8d 78 07             	lea    0x7(%eax),%edi
 935:	c1 ef 03             	shr    $0x3,%edi
 938:	83 c7 01             	add    $0x1,%edi
  if((prevp = freep) == 0){
 93b:	85 d2                	test   %edx,%edx
 93d:	0f 84 a3 00 00 00    	je     9e6 <malloc+0xc6>
 943:	8b 02                	mov    (%edx),%eax
 945:	8b 48 04             	mov    0x4(%eax),%ecx
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
 948:	39 cf                	cmp    %ecx,%edi
 94a:	76 74                	jbe    9c0 <malloc+0xa0>
 94c:	81 ff 00 10 00 00    	cmp    $0x1000,%edi
 952:	be 00 10 00 00       	mov    $0x1000,%esi
 957:	8d 1c fd 00 00 00 00 	lea    0x0(,%edi,8),%ebx
 95e:	0f 43 f7             	cmovae %edi,%esi
 961:	ba 00 80 00 00       	mov    $0x8000,%edx
 966:	81 ff ff 0f 00 00    	cmp    $0xfff,%edi
 96c:	0f 46 da             	cmovbe %edx,%ebx
 96f:	eb 10                	jmp    981 <malloc+0x61>
 971:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 978:	8b 02                	mov    (%edx),%eax
    if(p->s.size >= nunits){
 97a:	8b 48 04             	mov    0x4(%eax),%ecx
 97d:	39 cf                	cmp    %ecx,%edi
 97f:	76 3f                	jbe    9c0 <malloc+0xa0>
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 981:	39 05 88 0e 00 00    	cmp    %eax,0xe88
 987:	89 c2                	mov    %eax,%edx
 989:	75 ed                	jne    978 <malloc+0x58>
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
 98b:	83 ec 0c             	sub    $0xc,%esp
 98e:	53                   	push   %ebx
 98f:	e8 26 fc ff ff       	call   5ba <sbrk>
  if(p == (char*)-1)
 994:	83 c4 10             	add    $0x10,%esp
 997:	83 f8 ff             	cmp    $0xffffffff,%eax
 99a:	74 1c                	je     9b8 <malloc+0x98>
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
 99c:	89 70 04             	mov    %esi,0x4(%eax)
  free((void*)(hp + 1));
 99f:	83 ec 0c             	sub    $0xc,%esp
 9a2:	83 c0 08             	add    $0x8,%eax
 9a5:	50                   	push   %eax
 9a6:	e8 e5 fe ff ff       	call   890 <free>
  return freep;
 9ab:	8b 15 88 0e 00 00    	mov    0xe88,%edx
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
 9b1:	83 c4 10             	add    $0x10,%esp
 9b4:	85 d2                	test   %edx,%edx
 9b6:	75 c0                	jne    978 <malloc+0x58>
        return 0;
 9b8:	31 c0                	xor    %eax,%eax
 9ba:	eb 1c                	jmp    9d8 <malloc+0xb8>
 9bc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
 9c0:	39 cf                	cmp    %ecx,%edi
 9c2:	74 1c                	je     9e0 <malloc+0xc0>
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
 9c4:	29 f9                	sub    %edi,%ecx
 9c6:	89 48 04             	mov    %ecx,0x4(%eax)
        p += p->s.size;
 9c9:	8d 04 c8             	lea    (%eax,%ecx,8),%eax
        p->s.size = nunits;
 9cc:	89 78 04             	mov    %edi,0x4(%eax)
      }
      freep = prevp;
 9cf:	89 15 88 0e 00 00    	mov    %edx,0xe88
      return (void*)(p + 1);
 9d5:	83 c0 08             	add    $0x8,%eax
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 9d8:	8d 65 f4             	lea    -0xc(%ebp),%esp
 9db:	5b                   	pop    %ebx
 9dc:	5e                   	pop    %esi
 9dd:	5f                   	pop    %edi
 9de:	5d                   	pop    %ebp
 9df:	c3                   	ret    
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
 9e0:	8b 08                	mov    (%eax),%ecx
 9e2:	89 0a                	mov    %ecx,(%edx)
 9e4:	eb e9                	jmp    9cf <malloc+0xaf>
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
 9e6:	c7 05 88 0e 00 00 8c 	movl   $0xe8c,0xe88
 9ed:	0e 00 00 
 9f0:	c7 05 8c 0e 00 00 8c 	movl   $0xe8c,0xe8c
 9f7:	0e 00 00 
    base.s.size = 0;
 9fa:	b8 8c 0e 00 00       	mov    $0xe8c,%eax
 9ff:	c7 05 90 0e 00 00 00 	movl   $0x0,0xe90
 a06:	00 00 00 
 a09:	e9 3e ff ff ff       	jmp    94c <malloc+0x2c>
