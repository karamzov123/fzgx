#include "types.h"

extern void fn_8005912C(u32, void *, void *, void *);
extern u32 lbl_12_bss_1B9A8[2];

void fn_12_34DE4(void *arg0, void *arg1, void *arg2) {
    fn_8005912C(2, arg0, arg1, arg2);
    lbl_12_bss_1B9A8[0] = (u32)arg0;
}
