#include "types.h"

extern u32 fn_80046D94(u32, u32, u32, u32);

void fn_8004E324(u32 arg0, u32 arg1, u32 arg2) {
    fn_80046D94(arg2, arg1, (arg0 + 48), (arg0 + 50));
}
