8250 // PC keyboard interface constants
8251 
8252 #define KBSTATP         0x64    // kbd controller status port(I)
8253 #define KBS_DIB         0x01    // kbd data in buffer
8254 #define KBDATAP         0x60    // kbd data port(I)
8255 
8256 #define NO              0
8257 
8258 #define SHIFT           (1<<0)
8259 #define CTL             (1<<1)
8260 #define ALT             (1<<2)
8261 
8262 #define CAPSLOCK        (1<<3)
8263 #define NUMLOCK         (1<<4)
8264 #define SCROLLLOCK      (1<<5)
8265 
8266 #define E0ESC           (1<<6)
8267 
8268 // Special keycodes
8269 #define KEY_HOME        0xE0
8270 #define KEY_END         0xE1
8271 #define KEY_UP          0xE2
8272 #define KEY_DN          0xE3
8273 #define KEY_LF          0xE4
8274 #define KEY_RT          0xE5
8275 #define KEY_PGUP        0xE6
8276 #define KEY_PGDN        0xE7
8277 #define KEY_INS         0xE8
8278 #define KEY_DEL         0xE9
8279 
8280 // C('A') == Control-A
8281 #define C(x) (x - '@')
8282 
8283 static uchar shiftcode[256] =
8284 {
8285   [0x1D] CTL,
8286   [0x2A] SHIFT,
8287   [0x36] SHIFT,
8288   [0x38] ALT,
8289   [0x9D] CTL,
8290   [0xB8] ALT
8291 };
8292 
8293 static uchar togglecode[256] =
8294 {
8295   [0x3A] CAPSLOCK,
8296   [0x45] NUMLOCK,
8297   [0x46] SCROLLLOCK
8298 };
8299 
8300 static uchar normalmap[256] =
8301 {
8302   NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  // 0x00
8303   '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
8304   'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 0x10
8305   'o',  'p',  '[',  ']',  '\n', NO,   'a',  's',
8306   'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 0x20
8307   '\'', '`',  NO,   '\\', 'z',  'x',  'c',  'v',
8308   'b',  'n',  'm',  ',',  '.',  '/',  NO,   '*',  // 0x30
8309   NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
8310   NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
8311   '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
8312   '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
8313   [0x9C] '\n',      // KP_Enter
8314   [0xB5] '/',       // KP_Div
8315   [0xC8] KEY_UP,    [0xD0] KEY_DN,
8316   [0xC9] KEY_PGUP,  [0xD1] KEY_PGDN,
8317   [0xCB] KEY_LF,    [0xCD] KEY_RT,
8318   [0x97] KEY_HOME,  [0xCF] KEY_END,
8319   [0xD2] KEY_INS,   [0xD3] KEY_DEL
8320 };
8321 
8322 static uchar shiftmap[256] =
8323 {
8324   NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
8325   '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
8326   'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
8327   'O',  'P',  '{',  '}',  '\n', NO,   'A',  'S',
8328   'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
8329   '"',  '~',  NO,   '|',  'Z',  'X',  'C',  'V',
8330   'B',  'N',  'M',  '<',  '>',  '?',  NO,   '*',  // 0x30
8331   NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
8332   NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
8333   '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
8334   '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
8335   [0x9C] '\n',      // KP_Enter
8336   [0xB5] '/',       // KP_Div
8337   [0xC8] KEY_UP,    [0xD0] KEY_DN,
8338   [0xC9] KEY_PGUP,  [0xD1] KEY_PGDN,
8339   [0xCB] KEY_LF,    [0xCD] KEY_RT,
8340   [0x97] KEY_HOME,  [0xCF] KEY_END,
8341   [0xD2] KEY_INS,   [0xD3] KEY_DEL
8342 };
8343 
8344 
8345 
8346 
8347 
8348 
8349 
8350 static uchar ctlmap[256] =
8351 {
8352   NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
8353   NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
8354   C('Q'),  C('W'),  C('E'),  C('R'),  C('T'),  C('Y'),  C('U'),  C('I'),
8355   C('O'),  C('P'),  NO,      NO,      '\r',    NO,      C('A'),  C('S'),
8356   C('D'),  C('F'),  C('G'),  C('H'),  C('J'),  C('K'),  C('L'),  NO,
8357   NO,      NO,      NO,      C('\\'), C('Z'),  C('X'),  C('C'),  C('V'),
8358   C('B'),  C('N'),  C('M'),  NO,      NO,      C('/'),  NO,      NO,
8359   [0x9C] '\r',      // KP_Enter
8360   [0xB5] C('/'),    // KP_Div
8361   [0xC8] KEY_UP,    [0xD0] KEY_DN,
8362   [0xC9] KEY_PGUP,  [0xD1] KEY_PGDN,
8363   [0xCB] KEY_LF,    [0xCD] KEY_RT,
8364   [0x97] KEY_HOME,  [0xCF] KEY_END,
8365   [0xD2] KEY_INS,   [0xD3] KEY_DEL
8366 };
8367 
8368 
8369 
8370 
8371 
8372 
8373 
8374 
8375 
8376 
8377 
8378 
8379 
8380 
8381 
8382 
8383 
8384 
8385 
8386 
8387 
8388 
8389 
8390 
8391 
8392 
8393 
8394 
8395 
8396 
8397 
8398 
8399 
