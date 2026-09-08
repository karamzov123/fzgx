#include "types.h"

struct fn_80050670_Arg1 {
    u32 unk_0;
};
struct fn_80050670_lbl_8012D9B8 {
    u32 unk_0[1];
};

extern struct fn_80050670_lbl_8012D9B8 lbl_8012D9B8[];
extern u32 lbl_8012D9BC[];

u32 fn_80050670(u32 arg0, struct fn_80050670_Arg1 *arg1) {
    arg1->unk_0 = (u32)((u8 *)&lbl_8012D9BC + (arg0 * 12288));
    return lbl_8012D9B8[0].unk_0[arg0];
}
