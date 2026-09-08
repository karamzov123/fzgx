#include "types.h"

typedef struct {
    u8 unk[0xb];
    u8 value;
} Entry;

extern Entry *lbl_1_bss_53F8[34];
extern s16 lbl_1_bss_25C48[46];

u8 fn_1_3F264(u32 index) {
    u32 masked = index & 0xff;
    Entry *entry = lbl_1_bss_53F8[masked];

    if (entry != 0) {
        return entry->value;
    }

    return (u8)lbl_1_bss_25C48[masked];
}
