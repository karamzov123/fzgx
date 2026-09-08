#include "rel/main_rel/globals.h"
#include "rel/main_rel/camera.h"

extern void fn_1_A6FE8(void);
extern void fn_1_8D08(Obj_1_bss_F6C_Target *value);

// Initialize camera state before passing the shared camera object onward.
void fn_1_8AC0(void) {
    fn_1_A6FE8();
    fn_1_8D08(lbl_1_bss_F6C);
}
