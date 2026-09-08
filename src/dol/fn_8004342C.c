#include "types.h"

extern u32 fn_80043460(u32);
extern u32 fn_80043798(void);

struct fn_8004342C_Arg0 {
    u8 pad_0[0x9A];
    s16 unk_9A;
};

void fn_8004342C(struct fn_8004342C_Arg0 *arg0) {
    if (arg0->unk_9A == 1) {
    fn_80043460((u32)arg0);
    } else {
    fn_80043798();
    }
}
