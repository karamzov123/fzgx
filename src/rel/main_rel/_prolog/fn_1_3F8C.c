#include "types.h"

typedef struct Fn1_3F8C_Entry Fn1_3F8C_Entry;

struct Fn1_3F8C_Entry {
    u32 arg0;
    u32 arg1;
    Fn1_3F8C_Entry *next;
    Fn1_3F8C_Entry *prev;
    s32 result;
    u32 stamp;
    u32 reserved18;
    u32 reserved1c;
    u32 arg3;
};

typedef struct Fn1_3F8C_Table {
    Fn1_3F8C_Entry *head;
    u8 entries[0x14];
} Fn1_3F8C_Table;

extern Fn1_3F8C_Table *lbl_1_bss_DAC;
extern u32 lbl_1_bss_DB0[2];

s32 fn_1_3F8C(u32 arg3, u32 arg0, u32 arg1, u32 index) {
    Fn1_3F8C_Table *table = lbl_1_bss_DAC;
    Fn1_3F8C_Entry *entry = table->head;
    u8 *slot;
    Fn1_3F8C_Entry *link;

    if (entry == 0) {
        return -1;
    }

    table->head = entry->next;
    slot = (u8 *)table + index * 4;
    link = *(Fn1_3F8C_Entry **)(slot + 0x18);
    entry->next = link->next;
    entry->prev = *(Fn1_3F8C_Entry **)(slot + 0x18);
    entry->arg1 = arg1;
    entry->arg0 = arg0;
    entry->reserved18 = 0;
    entry->reserved1c = 0;
    entry->arg3 = arg3;
    entry->stamp = lbl_1_bss_DB0[0];

    if (entry->next != 0) {
        entry->next->prev = entry;
    }

    link = *(Fn1_3F8C_Entry **)(slot + 0x18);
    link->next = entry;
    return entry->result;
}

