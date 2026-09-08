#include "types.h"

typedef struct {
    u8 pad[0xa8];
    s16 value;
    u8 tail[0x152];
} CameraEntry;

extern CameraEntry *lbl_1_bss_F6C;

void fn_1_713C(u8 index, s16 value) {
    CameraEntry *entries = lbl_1_bss_F6C;

    entries[index].value = value;
}
