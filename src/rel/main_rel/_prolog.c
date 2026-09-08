#include "types.h"
#include "rel/main_rel/globals.h"

extern struct ArenaData lbl_1_data_8;
extern u32 OSGetArenaLo(void);
extern u32 OSGetArenaHi(void);
extern void OSSetArenaLo(u32 arena_lo);
extern u16 lbl_1_bss_990[28];
extern u8 lbl_1_bss_D78;
extern u8 lbl_1_bss_DA6;
extern u32 lbl_1_bss_DB0[2];
extern u32 lbl_1_bss_DA8;
extern void fn_1_43A4(void);
extern u32 lbl_1_data_2C70;
extern void fn_80008E84(u32 value);
extern u32 lbl_1_bss_DAC;

/* fzgx:begin fn_1_668 */
struct ArenaData {
    u32 *arena_lo;
    u32 arena_lo_size;
    u32 *aligned_lo;
    u32 aligned_lo_size;
};

void fn_1_668(void) {
    u32 arena_lo;
    u32 aligned_lo;

    arena_lo = OSGetArenaLo();
    OSGetArenaHi();
    *lbl_1_data_8.arena_lo = arena_lo;
    aligned_lo = (arena_lo + lbl_1_data_8.arena_lo_size + 0x1f) & ~0x1f;
    *lbl_1_data_8.aligned_lo = aligned_lo;
    aligned_lo = (aligned_lo + lbl_1_data_8.aligned_lo_size + 0x1f) & ~0x1f;
    OSSetArenaLo(aligned_lo);
}
/* fzgx:end fn_1_668 */

/* fzgx:begin fn_1_924 */
extern u16 lbl_1_bss_96A;
extern u32 fn_1_37E98(void);

u32 fn_1_924(void) {
    lbl_1_bss_96A = 36;
    return fn_1_37E98();
}
/* fzgx:end fn_1_924 */

/* fzgx:begin fn_1_950 */
// fn_1_950: empty in retail (single blr).
void fn_1_950(void) {
}
/* fzgx:end fn_1_950 */

/* fzgx:begin fn_1_954 */
// fn_1_954: empty in retail (single blr).
void fn_1_954(void) {
}
/* fzgx:end fn_1_954 */

/* fzgx:begin fn_1_958 */
// fn_1_958: empty in retail (single blr).
void fn_1_958(void) {
}
/* fzgx:end fn_1_958 */

/* fzgx:begin fn_1_E78 */
extern u32 fn_1_3BC0(u32);
extern u32 fn_1_A59AC(void);

void fn_1_E78(void) {
    u32 t0;
    t0 = fn_1_A59AC();
    fn_1_3BC0(t0);
}
/* fzgx:end fn_1_E78 */

/* fzgx:begin fn_1_1280 */
extern u32 lbl_1_bss_96C;

void fn_1_1280(u32 arg0) {
    lbl_1_bss_96C = arg0;
}
/* fzgx:end fn_1_1280 */

/* fzgx:begin fn_1_128C */
extern u32 camera_set_state_flag(u32);
extern u32 fn_1_D31E0(u32);

u32 fn_1_128C(void) {
    u32 t0;
    t0 = camera_set_state_flag(0);
    return fn_1_D31E0(t0);
}
/* fzgx:end fn_1_128C */

/* fzgx:begin fn_1_12FC */
// fn_1_12FC: empty in retail (single blr).
void fn_1_12FC(void) {
}
/* fzgx:end fn_1_12FC */

/* fzgx:begin fn_1_1300 */
// fn_1_1300: empty in retail (single blr).
void fn_1_1300(void) {
}
/* fzgx:end fn_1_1300 */

/* fzgx:begin fn_1_3BC0 */
void fn_1_3BC0(void) {
    lbl_1_bss_990[0] = 0;
    lbl_1_bss_990[1] = 0;
    lbl_1_bss_990[2] = 0;
    lbl_1_bss_990[3] = 0;
}
/* fzgx:end fn_1_3BC0 */

/* fzgx:begin fn_1_3CC4 */
void fn_1_3CC4(void) {
    if (lbl_1_bss_D78 == 0) {
        return;
    }
    if (lbl_1_bss_DA6 == 0) {
        lbl_1_bss_DA6 = 1;
    }
}
/* fzgx:end fn_1_3CC4 */

/* fzgx:begin fn_1_3F8C */
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

// Retrieves an entry from the free list and links it into the indexed list.
s32 fn_1_3F8C(u32 arg3, u32 arg0, u32 arg1, u32 index) {
    Fn1_3F8C_Table *table = (Fn1_3F8C_Table *)lbl_1_bss_DAC;
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
/* fzgx:end fn_1_3F8C */

/* fzgx:begin fn_1_41A8 */
typedef struct Node Node;
struct Node {
    u32 field_0;
    void *field_4;
    Node *next;
    u32 field_c;
    u32 field_10;
    u32 field_14;
    void (*callback)(void *, u32);
};

typedef struct List List;
struct List {
    u32 field_0;
    u32 field_4;
    Node *head;
};

typedef struct Manager Manager;
struct Manager {
    u32 field_0;
    u32 field_4;
    Node *head;
};

void fn_1_41A8(void) {
    Manager *entry;
    Manager *manager;
    u32 *dac;
    u32 *da8;
    int i;
    Node *node;
    void (*callback)(void *, u32);

    dac = &lbl_1_bss_DAC;
    manager = (Manager *)*dac;
    if (manager != 0) {
        da8 = &lbl_1_bss_DA8;
        entry = manager;
        i = 0;
        while (i < 32) {
            node = (*(List **)((u8 *)entry + 0x18))->head;
            while (node != 0) {
                callback = node->callback;
                manager->head = node;
                *dac = (u32)manager;
                *da8 = (u32)manager;
                if (callback != 0) {
                    callback(node->field_4, 1);
                }
                *dac = (u32)manager;
                *da8 = (u32)manager;
                node = node->next;
                fn_1_43A4();
            }
            i++;
            entry = (Manager *)((u8 *)entry + 4);
        }
        manager->head = 0;
        lbl_1_bss_DAC = (u32)manager;
        lbl_1_bss_DA8 = 0;
    }
}
/* fzgx:end fn_1_41A8 */

/* fzgx:begin fn_1_4370 */
// fn_1_4370: empty in retail (single blr).
void fn_1_4370(void) {
}
/* fzgx:end fn_1_4370 */

/* fzgx:begin fn_1_4374 */
typedef struct Node Node;
struct Node {
    u32 field_0;
    u32 field_4;
    Node *next;
    Node *prev;
};

void fn_1_4374(Node **list, Node *node) {
    Node *prev;
    Node *next;

    next = node->next;
    prev = node->prev;
    prev->next = next;
    if (next != 0) {
        next->prev = prev;
    }
    node->next = *list;
    node->field_0 = 0;
    *list = node;
}
/* fzgx:end fn_1_4374 */

/* fzgx:begin fn_1_43E8 noprologue */
#include "types.h"

extern u32 lbl_1_bss_DB0;

void fn_1_43E8(u32 arg0) {
    lbl_1_bss_DB0 = arg0;
}
/* fzgx:end fn_1_43E8 */

/* fzgx:begin fn_1_43F4 noprologue */
#include "types.h"

extern u32 lbl_1_bss_DB0;

void fn_1_43F4(void) {
    lbl_1_bss_DB0 = 0;
}
/* fzgx:end fn_1_43F4 */

/* fzgx:begin fn_1_446C */
void fn_1_446C(void) {
    fn_80008E84(lbl_1_data_2C70);
    lbl_1_data_2C70 = (u32)-1;
}
/* fzgx:end fn_1_446C */
