#include "types.h"

extern void fn_80059B44(void);
extern void fn_80059AB4(void);

typedef struct Fn8004B0EC {
    s8 unk00;
    s8 unk01;
    s8 unk02;
    s8 unk03[0x3e];
    s8 unk41;
    s8 unk42;
    s8 unk43;
    s8 unk44;
    s8 unk45;
} Fn8004B0EC;

void fn_8004B0EC(Fn8004B0EC *self) {
    fn_80059B44();
    if (self->unk01 == 2 && self->unk02 == 1) {
        self->unk44 = 1;
        if (self->unk43 == 1) {
            self->unk43 = 0;
        }
    } else {
        self->unk01 = 1;
    }
    fn_80059AB4();
    fn_80059B44();
    if (self->unk45 == 1) {
        self->unk42 = 1;
    }
    self->unk41 = 0;
    fn_80059AB4();
}
