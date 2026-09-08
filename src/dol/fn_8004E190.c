#include "types.h"

struct Fn8004E190 {
    u8 pad_0[0x10];
    u32 field_10;
};

u32 fn_8004E190(struct Fn8004E190 *arg) {
    return arg->field_10;
}
