#include "types.h"

typedef struct {
    u8 pad[0x10];
    u8 unk_10;
} Obj_1_bss_1018;

extern Obj_1_bss_1018 lbl_1_bss_1018;

u8* fn_1_D99C(void) {
    return &lbl_1_bss_1018.unk_10;
}
