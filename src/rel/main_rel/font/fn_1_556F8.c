#include "rel/main_rel/globals.h"

extern f32 lbl_1_bss_6C7A0;
extern s32 fn_1_54F5C(u8 *data, f32 value, f32 scale);
extern void fn_1_563E4(void *data);
extern void fn_80077F8C(void *value);
extern void fn_1_56530(void);

typedef struct {
    u8 unk_00[8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    f32 unk_14;
} Fn1_556F8Object;

/* Finishes the current font operation after testing its scale threshold. */
void fn_1_556F8(Fn1_556F8Object *value) {
    if (fn_1_54F5C((u8 *)&value->unk_08, value->unk_14, lbl_1_bss_6C7A0)) {
        fn_1_563E4(&lbl_1_bss_6C7A4);
        fn_80077F8C(value);
        fn_1_56530();
        return;
    }

    fn_1_56530();
}
