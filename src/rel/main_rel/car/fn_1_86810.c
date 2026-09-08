#include "types.h"

typedef struct {
    u8 pad_0[0x49c];
    void *object;
    u8 pad_4a0[0x180];
} Entry;

typedef struct {
    u8 pad_0[0x115];
    u8 value;
} Object;

extern Entry *lbl_1_bss_6D838;

u8 fn_1_86810(int index) {
    return ((Object *)(lbl_1_bss_6D838[index].object))->value;
}
