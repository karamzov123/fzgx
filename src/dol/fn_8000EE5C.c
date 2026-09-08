#include "types.h"

extern u32 __OSSavedRegionEnd;
extern u32 __OSSavedRegionStart;

struct fn_8000EE5C_Arg0 {
    u32 unk_0;
};
struct fn_8000EE5C_Arg1 {
    u32 unk_0;
};

void fn_8000EE5C(struct fn_8000EE5C_Arg0 *arg0, struct fn_8000EE5C_Arg1 *arg1) {
    arg0->unk_0 = __OSSavedRegionStart;
    arg1->unk_0 = __OSSavedRegionEnd;
}
