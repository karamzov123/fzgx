#include "types.h"
#include "dol/globals.h"

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 level);

typedef struct {
    u8 pad_0[0x4];
    s32 unk_4;
    u8 pad_8[0x104];
    u32 unk_10C;
} Fn8002A7C4Entry;

int fn_8002A7C4(s32 index, void *value) {
    Fn8002A7C4Entry *entry;
    u32 new_value;
    u32 level;

    entry = (Fn8002A7C4Entry *)&lbl_80177960 + index;
    level = OSDisableInterrupts();
    if (entry->unk_4 == -1) {
        return -1;
    }
    if (value != 0) {
        new_value = (u32)value;
    } else {
        new_value = (u32)0x8000 << 16;
    }
    entry->unk_10C = new_value;
    OSRestoreInterrupts(level);
    return 0;
}
