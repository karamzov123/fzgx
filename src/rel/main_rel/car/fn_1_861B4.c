#include "types.h"

extern u32 lbl_1_bss_6D838;
extern u8 lbl_1_bss_6D84A[6];
extern void fn_1_C489C(void *);
extern void fn_1_C40A0(void *);

void fn_1_861B4(void) {
    u32 i;
    u8 *entry;
    u8 *count;

    entry = (u8 *)lbl_1_bss_6D838;
    count = lbl_1_bss_6D84A;
    i = 0;
    while (i < (s8)*count) {
        if (*(u32 *)entry & 0x04000000) {
            fn_1_C489C(entry);
        }
        i++;
        entry += 0x620;
    }
    fn_1_C40A0((void *)lbl_1_bss_6D838);
}
