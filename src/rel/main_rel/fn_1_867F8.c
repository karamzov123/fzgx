#include "types.h"

typedef struct {
    u8 pad[0x184];
    f32 value;
    u8 tail[0x498];
} Entry;

extern Entry *lbl_1_bss_6D838;

f32 fn_1_867F8(int index) {
    return lbl_1_bss_6D838[index].value;
}
