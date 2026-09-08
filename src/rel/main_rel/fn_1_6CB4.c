#include "types.h"

typedef struct {
    u8 pad[2];
    s16 value;
} Result;

extern u32 lbl_1_bss_F68;
extern Result *lbl_1_bss_F6C;

void fn_1_6CB4(s16 value) {
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F6C->value = value;
    }
}
