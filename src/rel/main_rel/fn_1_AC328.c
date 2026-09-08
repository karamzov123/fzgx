#include "types.h"

typedef struct Fn1AC328State {
    u8 pad_00[0x2];
    u16 value;
    u8 pad_04[0x94];
} Fn1AC328State;

typedef struct Fn1AC328Target {
    u8 id;
    u8 pad_01[0x23];
    Fn1AC328State *state;
} Fn1AC328Target;

extern u8 lbl_1_data_3C7C4[216];

extern void fn_80083DB0(void *dst, void *src);
extern void fn_80083D40(void *dst, void *src);
extern void fn_8002FB04(u8 id, void *data, int zero);

void fn_1_AC328(Fn1AC328Target *target) {
    u8 data[0x40];

    fn_80083DB0(data, (u8 *)target->state + 0x98);
    fn_80083D40(data, lbl_1_data_3C7C4);
    target->state->value = 300;
    fn_8002FB04(target->id, data, 0);
}
