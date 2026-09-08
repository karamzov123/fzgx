#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Return the camera object's field at offset 0x18.
u32* fn_1_D9BC(void) {
    return &lbl_1_bss_1018.unk_18;
}
