#include "types.h"

extern f32 lbl_1_bss_6C7A0;
extern u8 lbl_1_bss_6C7A4[40];
extern s32 fn_1_54F5C(u8 *data, f32 value, f32 scale);
extern void fn_1_563E4(u8 *data);
extern void fn_80077F8C(void *value);
extern void fn_1_56530(void);

typedef struct {
    u8 unk_00[8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    f32 value_14;
} Fn1_556F8Object;

void fn_1_556F8(Fn1_556F8Object *value) {
    if (fn_1_54F5C((u8 *)value + 8, value->value_14, lbl_1_bss_6C7A0)) {
        fn_1_563E4(lbl_1_bss_6C7A4);
        fn_80077F8C(value);
        fn_1_56530();
        return;
    }
    fn_1_56530();
}
