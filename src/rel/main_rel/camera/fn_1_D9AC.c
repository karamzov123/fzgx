#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns a pointer to the camera object's field at offset 0x14.
u32* fn_1_D9AC(void) {
    return &lbl_1_bss_1018.unk_14;
}
