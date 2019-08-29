8400 #include "types.h"
8401 #include "x86.h"
8402 #include "defs.h"
8403 #include "kbd.h"
8404 
8405 int
8406 kbdgetc(void)
8407 {
8408   static uint shift;
8409   static uchar *charcode[4] = {
8410     normalmap, shiftmap, ctlmap, ctlmap
8411   };
8412   uint st, data, c;
8413 
8414   st = inb(KBSTATP);
8415   if((st & KBS_DIB) == 0)
8416     return -1;
8417   data = inb(KBDATAP);
8418 
8419   if(data == 0xE0){
8420     shift |= E0ESC;
8421     return 0;
8422   } else if(data & 0x80){
8423     // Key released
8424     data = (shift & E0ESC ? data : data & 0x7F);
8425     shift &= ~(shiftcode[data] | E0ESC);
8426     return 0;
8427   } else if(shift & E0ESC){
8428     // Last character was an E0 escape; or with 0x80
8429     data |= 0x80;
8430     shift &= ~E0ESC;
8431   }
8432 
8433   shift |= shiftcode[data];
8434   shift ^= togglecode[data];
8435   c = charcode[shift & (CTL | SHIFT)][data];
8436   if(shift & CAPSLOCK){
8437     if('a' <= c && c <= 'z')
8438       c += 'A' - 'a';
8439     else if('A' <= c && c <= 'Z')
8440       c += 'a' - 'A';
8441   }
8442   return c;
8443 }
8444 
8445 void
8446 kbdintr(void)
8447 {
8448   consoleintr(kbdgetc);
8449 }
