#include "types.h"

extern int fn_1_3F164(void);
extern u32 lbl_1_bss_4C688[2048];

typedef struct MainEntry {
    int active;
    u8 pad[0x74];
    void (*callback)(struct MainEntry *);
    u32 pad_end;
} MainEntry;

void fn_1_4DE44(void) {
    int i;
    MainEntry *entry;

    if (fn_1_3F164() == 0) {
        entry = (MainEntry *)lbl_1_bss_4C688;
        for (i = 0; i < 0x40; i++, entry++) {
            if (entry->active != 0 && entry->callback != 0) {
                entry->callback(entry);
            }
        }
    }
}
