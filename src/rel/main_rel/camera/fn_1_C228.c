#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern void OSReport(const unsigned char *, ...);

// Reports the camera's current position values for debugging.
void fn_1_C228(void) {
    OSReport(&lbl_1_data_4128[0], lbl_1_bss_10D8.unk_10,
             lbl_1_bss_10D8.unk_14, lbl_1_bss_10D8.unk_18);
}
