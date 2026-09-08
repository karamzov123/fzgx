#include "types.h"

extern u8 lbl_1_rodata_CCDC[308];
extern void fn_1_14FD7C(u32 *data, u32 count);
extern void fn_80008BA8(u32 arg0, u32 *data, s32 offset);

typedef struct {
    u32 words[77];
} Data;

void fn_1_14FCE4(u32 arg0, s16 arg1) {
    Data data;

    data = *(Data *)lbl_1_rodata_CCDC;
    fn_1_14FD7C(data.words, 77);
    fn_80008BA8(arg0, data.words, (s32)arg1 << 2);
}
