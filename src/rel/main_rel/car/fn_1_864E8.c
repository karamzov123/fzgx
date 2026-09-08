#include "rel/main_rel/globals.h"

// Returns the indexed car record from the shared car table.
u32 fn_1_864E8(int index) {
    return ((u32 *)lbl_1_bss_6D838)[index * (0x620 / sizeof(u32))];
}
