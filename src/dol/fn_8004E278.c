#include "types.h"

extern u32 memset(u32, u32, u32);

struct fn_8004E278_Arg0 {
    u32 unk_0;
};

void fn_8004E278(struct fn_8004E278_Arg0 *arg0) {
    if ((u32)arg0 != 0) {
    arg0->unk_0 = 0;
    memset((u32)arg0, 0, 60);
    }
}
