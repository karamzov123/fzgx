#include "types.h"
#include "rel/main_rel/globals.h"

extern void fn_80074A7C(void);

// Stores the current value for the font subsystem and refreshes its state.
void fn_1_5638C(u32 value) {
    lbl_1_bss_6C7A4.unk_20 = value;
    fn_80074A7C();
}
