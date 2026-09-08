#include "types.h"

typedef struct {
    u8 pad[2];
    s16 value;
    u8 rest[0x1f8];
} Entry;

extern Entry *lbl_1_bss_F6C;

s16 fn_1_8628(u32 index) {
    return lbl_1_bss_F6C[(u8)index].value;
}
