#include "types.h"

typedef struct {
    u8 pad[0xA4];
    s16 value;
    u8 tail[0x156];
} Entry;

extern Entry *lbl_1_bss_F6C;

s16 fn_1_725C(u32 index) {
    return lbl_1_bss_F6C[(u8)index].value;
}
