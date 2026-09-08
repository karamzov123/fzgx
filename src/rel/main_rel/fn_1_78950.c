#include "types.h"

extern u8 lbl_1_bss_6D14C[92];
extern void fn_8006FDEC(void);
extern void fn_8006FEFC(void);
extern void fn_8006FD1C(void);

void fn_1_78950(void) {
    int i;

    lbl_1_bss_6D14C[0] = 1;
    for (i = 0; i < 2; i++) {
        fn_8006FDEC();
        fn_8006FEFC();
        fn_8006FD1C();
    }
}
