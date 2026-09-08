#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the selected camera entry value for an 8-bit camera index.
s16 fn_1_8628(u32 index) {
    return lbl_1_bss_F6C[(u8)index].unk_2;
}
