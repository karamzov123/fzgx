#include "types.h"

extern struct fn_16_9EC_lbl_16_bss_14C lbl_16_bss_14C;
extern struct fn_16_9EC_lbl_16_bss_150 lbl_16_bss_150;
extern u32 fn_1_3CC4(u32);
extern u32 fn_1_407C(u32);
extern u32 fn_1_435C(u32);

struct fn_16_9EC_lbl_16_bss_14C {
    u32 unk_0;
};
struct fn_16_9EC_lbl_16_bss_150 {
    u32 unk_0;
};

void fn_16_9EC(void) {
    u32 t0, t1, t3;
    t0 = fn_1_435C(lbl_16_bss_14C.unk_0);
    t1 = fn_1_407C(t0);
    fn_1_3CC4(t1);
    t3 = fn_1_435C(lbl_16_bss_150.unk_0);
    fn_1_407C(t3);
}
