#include "types.h"
#include "rel/option/globals.h"

extern s32 lbl_801A66B4;
extern void fn_1_1380F0(void *);
extern u8 fn_1_B7C00(void);
extern void fn_1_13ABA8(u32);

typedef void (*OptionFn)(void);

typedef struct {
    u8 pad_0[0xC];
    OptionFn unk_C[3];
    s32 unk_18;
    u8 pad_1C[0x44];
    OptionFn unk_60[1];
} Obj_4_AC58;

// Renders the active option overlay and dispatches the pending option action.
void fn_4_AC58(void) {
    Obj_4_AC58 *p = (Obj_4_AC58 *)&lbl_4_data_2F00;

    fn_1_1380F0((void *)p->unk_60[lbl_801A66B4]);

    if (fn_1_B7C00() != 0 || (s8)lbl_4_bss_5630.unk_22 == 2) {
        fn_1_13ABA8(0);
    } else {
        fn_1_13ABA8(0x28000000);
    }

    if (p->unk_18 >= 0) {
        p->unk_C[p->unk_18]();
    }
}
