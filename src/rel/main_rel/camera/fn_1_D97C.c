#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns the address of the camera state field at offset 0x8.
u32* fn_1_D97C(void) {
    return &lbl_1_bss_1018.unk_8;
}
