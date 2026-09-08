#include "types.h"

extern u8 *lbl_1_bss_6D838;
extern int fn_80007D58(void *);
extern void lbl_8006D784(void *);
extern void lbl_8006DD14(void *, void *);

void fn_1_8636C(int index, void *arg) {
    int offset = index * 0x620;

    if (fn_80007D58(lbl_1_bss_6D838 + offset + 0xec) != 0) {
        lbl_8006D784(arg);
    } else {
        lbl_8006DD14(lbl_1_bss_6D838 + offset + 0xec, arg);
    }
}
