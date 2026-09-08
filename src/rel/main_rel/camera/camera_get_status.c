#include "rel/main_rel/globals.h"

u32 fn_1_6514(u32);

// Return the camera status when a camera exists and is not marked inactive.
u32 camera_get_status(void) {
    Obj_1_bss_F68_Target *camera = lbl_1_bss_F68;
    u32 status;

    if (camera == 0) {
        status = 0;
    } else if ((camera->unk_0 & ((u32)1 << 31)) != 0) {
        status = 0;
    } else {
        status = camera->unk_4A;
    }

    return fn_1_6514(status);
}
