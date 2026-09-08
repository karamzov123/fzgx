#include "rel/main_rel/globals.h"

extern f32 lbl_1_bss_1040;
extern f32 lbl_1_bss_1044;

// Saves the two current camera parameters for later processing.
void fn_1_B85C(f32 first_parameter, f32 second_parameter) {
    lbl_1_bss_1040 = first_parameter;
    lbl_1_bss_1044 = second_parameter;
}
