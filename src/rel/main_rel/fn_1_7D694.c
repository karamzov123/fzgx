#include "types.h"

extern u32 lbl_1_bss_6D7E8[2];

typedef struct {
    u8 _pad4[4];
    s16 value;
} Entry;

typedef struct {
    u8 _pad32c[0x32c];
    Entry *entry;
} Object;

typedef struct {
    u8 _pad7[7];
    s8 value;
} GlobalEntry;

void fn_1_7D694(Object *obj) {
    Entry *entry = obj->entry;
    GlobalEntry *global = (GlobalEntry *)lbl_1_bss_6D7E8[0];
    if (entry->value == global->value) {
        return;
    }
}
