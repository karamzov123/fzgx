#include "types.h"

typedef struct {
    u8 pad_0[0x188];
    s16 value;
    u8 pad_18a[0x496];
} Entry;

extern Entry *lbl_1_bss_6D838;

s16 fn_1_8683C(int index) {
    return lbl_1_bss_6D838[index].value;
}
