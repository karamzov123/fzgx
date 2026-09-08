#include "types.h"

extern u32 fn_80008FB0(void);
extern u32 lbl_801A6734;

void fn_80008F88(void) {
    lbl_801A6734 = 1;
    fn_80008FB0();
}
