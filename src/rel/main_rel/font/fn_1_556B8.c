#include "types.h"

extern u8 lbl_1_bss_6C7A4[40];
extern void fn_1_563E4(u8 *data);
extern void fn_80077F8C(void *value);
extern void fn_1_56530(void);

void fn_1_556B8(void *value) {
    fn_1_563E4(lbl_1_bss_6C7A4);
    fn_80077F8C(value);
    fn_1_56530();
}
