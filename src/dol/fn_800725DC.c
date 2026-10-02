#include "dolphin/hw_regs.h"
#include "types.h"

struct fn_800725DC_lbl_8019E250 {
    u8 pad_0[0x30];
    u32 unk_30;
};

extern struct fn_800725DC_lbl_8019E250 lbl_8019E250;

#pragma opt_dead_assignments off
void fn_800725DC(u32 *arg0) {
    u32 value;
    u32 fifo;
    struct fn_800725DC_lbl_8019E250 *p;

    p = &lbl_8019E250;
    value = *arg0;
    fifo = GX_FIFO_BASE;
    if (p->unk_30 == value) {
        return;
    }
    p->unk_30 = value;
    *(volatile u8 *)fifo = 16; /* Hardware access must remain ordered. */
    *(volatile u32 *)fifo = 0x100c; /* Hardware access must remain ordered. */
    *(volatile u32 *)fifo = value; /* Hardware access must remain ordered. */
}
#pragma opt_dead_assignments reset
