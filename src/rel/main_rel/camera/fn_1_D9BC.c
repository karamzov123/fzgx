#include "types.h"

typedef struct {
    u8 pad_0[0x18];
    u32 unk_18;
} Obj_1_bss_1018;

extern Obj_1_bss_1018 lbl_1_bss_1018;

u32* fn_1_D9BC(void) {
    return &lbl_1_bss_1018.unk_18;
}
