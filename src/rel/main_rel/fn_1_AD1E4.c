#include "types.h"

typedef struct Fn1AD1E4A {
    u8 pad0[8];
    void *field8;
    void *fieldC;
} Fn1AD1E4A;

typedef struct Fn1AD1E4P {
    u8 pad0[8];
    u32 flags;
    u8 padC[0x88];
    void *field94;
    u8 pad98[0x20];
    void *fieldB8;
} Fn1AD1E4P;

typedef struct Fn1AD1E4B {
    u8 pad0[3];
    u8 state;
    u8 pad4[0x20];
    Fn1AD1E4P *field24;
} Fn1AD1E4B;

extern void fn_80083D6C(void *arg0, void *arg1, int arg2);
extern void fn_80008BA8(void *arg0, void *arg1, void *arg2);

void fn_1_AD1E4(Fn1AD1E4A *arg0, Fn1AD1E4B *arg1) {
    if ((arg1->field24->flags & 0x20) == 0) {
        if (arg0->field8 != 0) {
            fn_80083D6C((u8 *)arg1->field24->field94 + 0x24,
                        arg0->field8, 0x20);
        }
        fn_80008BA8((u8 *)arg1->field24->field94 + 0x2060,
                    arg0->fieldC, arg1->field24->fieldB8);
    }
    arg1->state = 3;
}
