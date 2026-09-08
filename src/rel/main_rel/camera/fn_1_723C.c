#include "types.h"

typedef struct {
    u8 pad[0xA8];
    s16 value;
    u8 tail[0x152];
} CameraEntry;

extern CameraEntry *lbl_1_bss_F6C;

s16 fn_1_723C(u32 index) {
    return lbl_1_bss_F6C[(u8)index].value;
}
