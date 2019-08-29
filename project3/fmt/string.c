7450 #include "types.h"
7451 #include "x86.h"
7452 
7453 void*
7454 memset(void *dst, int c, uint n)
7455 {
7456   if ((int)dst%4 == 0 && n%4 == 0){
7457     c &= 0xFF;
7458     stosl(dst, (c<<24)|(c<<16)|(c<<8)|c, n/4);
7459   } else
7460     stosb(dst, c, n);
7461   return dst;
7462 }
7463 
7464 int
7465 memcmp(const void *v1, const void *v2, uint n)
7466 {
7467   const uchar *s1, *s2;
7468 
7469   s1 = v1;
7470   s2 = v2;
7471   while(n-- > 0){
7472     if(*s1 != *s2)
7473       return *s1 - *s2;
7474     s1++, s2++;
7475   }
7476 
7477   return 0;
7478 }
7479 
7480 void*
7481 memmove(void *dst, const void *src, uint n)
7482 {
7483   const char *s;
7484   char *d;
7485 
7486   s = src;
7487   d = dst;
7488   if(s < d && s + n > d){
7489     s += n;
7490     d += n;
7491     while(n-- > 0)
7492       *--d = *--s;
7493   } else
7494     while(n-- > 0)
7495       *d++ = *s++;
7496 
7497   return dst;
7498 }
7499 
7500 // memcpy exists to placate GCC.  Use memmove.
7501 void*
7502 memcpy(void *dst, const void *src, uint n)
7503 {
7504   return memmove(dst, src, n);
7505 }
7506 
7507 int
7508 strncmp(const char *p, const char *q, uint n)
7509 {
7510   while(n > 0 && *p && *p == *q)
7511     n--, p++, q++;
7512   if(n == 0)
7513     return 0;
7514   return (uchar)*p - (uchar)*q;
7515 }
7516 
7517 char*
7518 strncpy(char *s, const char *t, int n)
7519 {
7520   char *os;
7521 
7522   os = s;
7523   while(n-- > 0 && (*s++ = *t++) != 0)
7524     ;
7525   while(n-- > 0)
7526     *s++ = 0;
7527   return os;
7528 }
7529 
7530 // Like strncpy but guaranteed to NUL-terminate.
7531 char*
7532 safestrcpy(char *s, const char *t, int n)
7533 {
7534   char *os;
7535 
7536   os = s;
7537   if(n <= 0)
7538     return os;
7539   while(--n > 0 && (*s++ = *t++) != 0)
7540     ;
7541   *s = 0;
7542   return os;
7543 }
7544 
7545 
7546 
7547 
7548 
7549 
7550 int
7551 strlen(const char *s)
7552 {
7553   int n;
7554 
7555   for(n = 0; s[n]; n++)
7556     ;
7557   return n;
7558 }
7559 
7560 
7561 
7562 
7563 
7564 
7565 
7566 
7567 
7568 
7569 
7570 
7571 
7572 
7573 
7574 
7575 
7576 
7577 
7578 
7579 
7580 
7581 
7582 
7583 
7584 
7585 
7586 
7587 
7588 
7589 
7590 
7591 
7592 
7593 
7594 
7595 
7596 
7597 
7598 
7599 
