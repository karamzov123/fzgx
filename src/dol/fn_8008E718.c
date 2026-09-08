#include "types.h"

extern u32 OSRestoreInterrupts(u32);

struct fn_8008E718_Arg0 {
    u32 unk_0;
};

void fn_8008E718(struct fn_8008E718_Arg0 *arg0) {
    OSRestoreInterrupts(arg0->unk_0);
}
