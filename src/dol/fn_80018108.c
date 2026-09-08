#include "types.h"

extern struct fn_80018108_IDShouldBe_801A68C4_T *IDShouldBe_801A68C4;
extern u32 fn_80016BF8(u32, u32, u32);
extern void fn_80018414(void);

struct fn_80018108_IDShouldBe_801A68C4_T {
    u8 pad_0[0x8];
    u8 unk_8;
};

void fn_80018108(void) {
    fn_80016BF8(IDShouldBe_801A68C4->unk_8, 10, (u32)fn_80018414);
}
