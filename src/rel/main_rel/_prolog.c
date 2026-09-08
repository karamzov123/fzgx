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

/* fzgx:begin fn_1_634 */
extern struct fn_1_634_lbl_1_bss_54 lbl_1_bss_54;
extern u32 fn_8006FFC4(u32);

struct fn_1_634_lbl_1_bss_54 {
    u32 unk_0;
};

void * fn_1_634(void) {
    fn_8006FFC4(lbl_1_bss_54.unk_0);
    return &lbl_1_bss_54;
}
/* fzgx:end fn_1_634 */

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

/* fzgx:begin fn_1_8D4 */
extern u32 fn_1_48780(void);
extern u32 lbl_801A66B4;
extern u32 lbl_801A66BC;

void fn_1_8D4(void) {
    lbl_801A66B4 = lbl_801A66BC;
    fn_1_48780();
}
/* fzgx:end fn_1_8D4 */

/* fzgx:begin fn_1_904 */
extern struct fn_1_904_lbl_1_bss_4 lbl_1_bss_4;

struct fn_1_904_lbl_1_bss_4 {
    u32 unk_0;
};

u32 fn_1_904(void) {
    return lbl_1_bss_4.unk_0;
}
/* fzgx:end fn_1_904 */

/* fzgx:begin fn_1_914 */
extern struct fn_1_914_lbl_1_bss_0 lbl_1_bss_0;

struct fn_1_914_lbl_1_bss_0 {
    u32 unk_0;
};

u32 fn_1_914(void) {
    return lbl_1_bss_0.unk_0;
}
/* fzgx:end fn_1_914 */

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

/* fzgx:begin fn_1_12B4 */
extern struct fn_1_12B4_lbl_1_bss_962 lbl_1_bss_962;
extern u32 OSReport(void *, ...);
extern u32 lbl_1_data_2A20;
extern u32 lbl_1_data_7C0;

struct fn_1_12B4_lbl_1_bss_962 {
    s16 unk_0;
};

void fn_1_12B4(void) {
    s16 v0;
    v0 = lbl_1_bss_962.unk_0;
    OSReport(&lbl_1_data_2A20, v0, ((u8 *)&lbl_1_data_7C0 + (v0 * 44)));
}
/* fzgx:end fn_1_12B4 */

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

/* fzgx:begin fn_1_3238 */
extern u32 fn_1_15659C(u32);
extern u32 fn_80008BEC(void *, u32, u32);
extern u8 lbl_1_bss_978[];

void fn_1_3238(void) {
    u8 *p_lbl_1_bss_978;
    u32 t9;
    p_lbl_1_bss_978 = (u8 *)&lbl_1_bss_978;
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 80), 0, 48);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 32), 0, 48);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 128), 0, 80);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 208), 0, 64);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 272), 0, 24);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 840), 0, 4);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 844), 0, 52);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 896), 0, 48);
    fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 944), 0, 48);
    t9 = fn_80008BEC(((u8 *)p_lbl_1_bss_978 + 24), 0, 8);
    fn_1_15659C(t9);
}
/* fzgx:end fn_1_3238 */

/* fzgx:begin fn_1_3BB0 */
extern u32 lbl_1_data_2B78;

u8 fn_1_3BB0(u32 arg0) {
    return *(u8 *)((u8 *)&lbl_1_data_2B78 + arg0);
}
/* fzgx:end fn_1_3BB0 */

/* fzgx:begin fn_1_3BC0 */
void fn_1_3BC0(void) {
    lbl_1_bss_990[0] = 0;
    lbl_1_bss_990[1] = 0;
    lbl_1_bss_990[2] = 0;
    lbl_1_bss_990[3] = 0;
}
/* fzgx:end fn_1_3BC0 */

/* fzgx:begin fn_1_3BDC */
extern u32 OSGetTick(u32);
extern u32 lbl_1_bss_D7C;

void fn_1_3BDC(u32 arg0) {
    u32 t0;
    t0 = OSGetTick(arg0);
    *(u32 *)((u8 *)&lbl_1_bss_D7C + (arg0 << 2)) = t0;
}
/* fzgx:end fn_1_3BDC */

/* fzgx:begin fn_1_3C78 */
extern u8 lbl_1_bss_D78;
extern u8 lbl_1_bss_DA4;

void fn_1_3C78(void) {
    if (lbl_1_bss_D78 == 0) { return; }
    lbl_1_bss_DA4 = 0;
}
/* fzgx:end fn_1_3C78 */

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

/* fzgx:begin fn_1_4010 noprologue */
#include "types.h"

extern struct fn_1_4010_lbl_1_bss_DAC lbl_1_bss_DAC;

struct fn_1_4010_lbl_1_bss_DAC {
    u32 unk_0;
};

u32 fn_1_4010(u32 arg0, u32 arg1) {
    u32 v0;
    v0 = (*(u32 *)((u8 *)lbl_1_bss_DAC.unk_0 + 4) + (arg0 * 36));
    *(u32 *)((u8 *)v0 + 24) = arg1;
    return v0;
}
/* fzgx:end fn_1_4010 */

/* fzgx:begin fn_1_4060 noprologue */
#include "types.h"

extern struct fn_1_4060_lbl_1_bss_DA8 lbl_1_bss_DA8;

struct fn_1_4060_lbl_1_bss_DA8 {
    u32 unk_0;
};

u32 fn_1_4060(void) {
    u32 v0;
    v0 = *(u32 *)((u8 *)lbl_1_bss_DA8.unk_0 + 8);
    *(u32 *)((u8 *)v0 + 0) = 0;
    return v0;
}
/* fzgx:end fn_1_4060 */

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

/* fzgx:begin fn_1_4310 noprologue */
#include "types.h"

extern struct fn_1_4310_lbl_1_bss_DAC lbl_1_bss_DAC;

struct fn_1_4310_lbl_1_bss_DAC {
    u32 unk_0;
};

void fn_1_4310(u32 arg0) {
    *(u32 *)((u8 *)lbl_1_bss_DAC.unk_0 + 16) = arg0;
}
/* fzgx:end fn_1_4310 */

/* fzgx:begin fn_1_4324 noprologue */
#include "types.h"

extern struct fn_1_4324_lbl_1_bss_DAC lbl_1_bss_DAC;

struct fn_1_4324_lbl_1_bss_DAC {
    u32 unk_0;
};

void fn_1_4324(u32 arg0) {
    *(u32 *)((u8 *)lbl_1_bss_DAC.unk_0 + 20) = arg0;
}
/* fzgx:end fn_1_4324 */

/* fzgx:begin fn_1_4338 noprologue */
#include "types.h"

extern struct fn_1_4338_lbl_1_bss_DAC lbl_1_bss_DAC;

struct fn_1_4338_lbl_1_bss_DAC {
    u32 unk_0;
};

u32 fn_1_4338(void) {
    return *(u32 *)((u8 *)lbl_1_bss_DAC.unk_0 + 20);
}
/* fzgx:end fn_1_4338 */

/* fzgx:begin fn_1_434C noprologue */
#include "types.h"

extern struct fn_1_434C_lbl_1_bss_DAC lbl_1_bss_DAC;

struct fn_1_434C_lbl_1_bss_DAC {
    u32 unk_0;
};

u32 fn_1_434C(void) {
    return lbl_1_bss_DAC.unk_0;
}
/* fzgx:end fn_1_434C */

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

/* fzgx:begin fn_1_4404 noprologue */
#include "types.h"

extern struct fn_1_4404_lbl_1_bss_DC0 lbl_1_bss_DC0;
extern u32 fn_80008E84(u32);
extern u32 lbl_1_data_2C70;

struct fn_1_4404_lbl_1_bss_DC0 {
    u32 unk_0;
};

void fn_1_4404(void) {
    u32 t0;
    t0 = fn_80008E84(lbl_1_bss_DC0.unk_0);
    lbl_1_data_2C70 = t0;
}
/* fzgx:end fn_1_4404 */

/* fzgx:begin fn_1_4438 noprologue */
#include "types.h"

extern struct fn_1_4438_lbl_1_bss_DC4 lbl_1_bss_DC4;
extern u32 fn_80008E84(u32);
extern u32 lbl_1_data_2C70;

struct fn_1_4438_lbl_1_bss_DC4 {
    u32 unk_0;
};

void fn_1_4438(void) {
    u32 t0;
    t0 = fn_80008E84(lbl_1_bss_DC4.unk_0);
    lbl_1_data_2C70 = t0;
}
/* fzgx:end fn_1_4438 */

/* fzgx:begin fn_1_446C */
void fn_1_446C(void) {
    fn_80008E84(lbl_1_data_2C70);
    lbl_1_data_2C70 = (u32)-1;
}
/* fzgx:end fn_1_446C */

/* fzgx:begin fn_1_44A4 */
extern struct fn_1_44A4_lbl_1_bss_DC4 lbl_1_bss_DC4;

struct fn_1_44A4_lbl_1_bss_DC4 {
    u32 unk_0;
};

u32 fn_1_44A4(void) {
    return lbl_1_bss_DC4.unk_0;
}
/* fzgx:end fn_1_44A4 */
