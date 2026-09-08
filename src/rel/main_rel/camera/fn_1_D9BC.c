#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

// Returns a pointer to the camera object's state field at offset 0x18.
u32* fn_1_D9BC(void) {
    return &lbl_1_bss_1018.unk_18;
}
