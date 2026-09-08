#include "types.h"

extern s16 fn_1_98104(void);
extern u32 *lbl_1_bss_6EAD0;
extern void fn_8004C698(u32 arg0);

void fn_1_9724C(void) {
    if (fn_1_98104() == 0) {
        fn_8004C698(lbl_1_bss_6EAD0[1]);
    }
}
