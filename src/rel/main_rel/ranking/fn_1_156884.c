#include "types.h"

typedef struct {
    u8 unk0[4];
    u32 field4;
    u8 unk8[0x48];
} Entry;

extern Entry lbl_1_bss_8F428[4];
extern void fn_1_1574E0(Entry *entry, u32 value);

void fn_1_156884(int index) {
    Entry *entry = &lbl_1_bss_8F428[index];
    u32 value = entry->field4;

    if (value + 0x10000 != 0xffff) {
        fn_1_1574E0(entry, value);
    }
}
