#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_155F9C */
extern u32 lbl_1_bss_8F3E0;

void fn_1_155F9C(u32 value) {
    lbl_1_bss_8F3E0 = value;
}
/* fzgx:end fn_1_155F9C */

/* fzgx:begin fn_1_1564D0 */
extern void fn_1_451C(void);
extern void fn_1_A1340(void);
extern s16 lbl_1_bss_962;
extern s16 lbl_1_bss_96A;

void fn_1_1564D0(void) {
    fn_1_451C();
    fn_1_A1340();
    if (lbl_1_bss_962 != 0x7d) {
        lbl_1_bss_96A = 0x7d;
    }
}
/* fzgx:end fn_1_1564D0 */

/* fzgx:begin fn_1_156530 */
// fn_1_156530: empty in retail (single blr).
void fn_1_156530(void) {
}
/* fzgx:end fn_1_156530 */

/* fzgx:begin fn_1_156554 */
// fn_1_156554: empty in retail (single blr).
void fn_1_156554(void) {
}
/* fzgx:end fn_1_156554 */

/* fzgx:begin fn_1_156558 */
// fn_1_156558: empty in retail (single blr).
void fn_1_156558(void) {
}
/* fzgx:end fn_1_156558 */

/* fzgx:begin fn_1_15655C */
// fn_1_15655C: empty in retail (single blr).
void fn_1_15655C(void) {
}
/* fzgx:end fn_1_15655C */

/* fzgx:begin fn_1_156560 */
// fn_1_156560: empty in retail (single blr).
void fn_1_156560(void) {
}
/* fzgx:end fn_1_156560 */

/* fzgx:begin fn_1_156564 */
// fn_1_156564: empty in retail (single blr).
void fn_1_156564(void) {
}
/* fzgx:end fn_1_156564 */

/* fzgx:begin fn_1_156568 */
// fn_1_156568: empty in retail (single blr).
void fn_1_156568(void) {
}
/* fzgx:end fn_1_156568 */

/* fzgx:begin fn_1_15657C */
// fn_1_15657C: empty in retail (single blr).
void fn_1_15657C(void) {
}
/* fzgx:end fn_1_15657C */

/* fzgx:begin fn_1_156580 */
// fn_1_156580: empty in retail (single blr).
void fn_1_156580(void) {
}
/* fzgx:end fn_1_156580 */

/* fzgx:begin fn_1_156584 */
// fn_1_156584: empty in retail (single blr).
void fn_1_156584(void) {
}
/* fzgx:end fn_1_156584 */

/* fzgx:begin fn_1_156588 */
// fn_1_156588: empty in retail (single blr).
void fn_1_156588(void) {
}
/* fzgx:end fn_1_156588 */

/* fzgx:begin fn_1_15658C */
// fn_1_15658C: empty in retail (single blr).
void fn_1_15658C(void) {
}
/* fzgx:end fn_1_15658C */

/* fzgx:begin fn_1_156590 */
// fn_1_156590: empty in retail (single blr).
void fn_1_156590(void) {
}
/* fzgx:end fn_1_156590 */

/* fzgx:begin fn_1_156594 */
// fn_1_156594: empty in retail (single blr).
void fn_1_156594(void) {
}
/* fzgx:end fn_1_156594 */

/* fzgx:begin fn_1_156598 */
// fn_1_156598: empty in retail (single blr).
void fn_1_156598(void) {
}
/* fzgx:end fn_1_156598 */

/* fzgx:begin fn_1_15665C */
extern u8 lbl_1_bss_8F428[320];
extern f32 lbl_1_rodata_D8C8[18];
extern void fn_80008BEC(void *, int, u32);
extern void fn_1_1568C4(void *);

typedef struct Entry {
    u32 unk_00;
    void *owner;
    s32 unk_08;
    s32 unk_0c;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1c;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2c;
    u32 unk_30;
    f32 value_34;
    u8 unk_38[0x18];
} Entry;

void fn_1_15665C(void *owner, s32 index) {
    Entry *entry;

    entry = (Entry *)lbl_1_bss_8F428 + index;
    fn_80008BEC(entry, 0, 0x50);
    entry->unk_00 = 0;
    entry->owner = owner;
    entry->unk_08 = -1;
    entry->unk_0c = -1;
    entry->unk_10 = -1;
    entry->unk_14 = -1;
    entry->unk_1c = -1;
    entry->unk_20 = -1;
    entry->unk_18 = -1;
    entry->unk_24 = -1;
    entry->unk_28 = -1;
    entry->unk_2c = -1;
    entry->value_34 = lbl_1_rodata_D8C8[0];
    fn_1_1568C4(entry);
}
/* fzgx:end fn_1_15665C */

/* fzgx:begin fn_1_156884 */
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
/* fzgx:end fn_1_156884 */

/* fzgx:begin fn_1_1569A0 */
extern void fn_8006B7B4(void *);

typedef struct {
    u32 flags;
    u8 _pad04[4];
    void *data;
} State;

void fn_1_1569A0(State *state) {
    if (state->flags & 1) {
        fn_8006B7B4(state->data);
        state->flags &= ~1;
    }
}
/* fzgx:end fn_1_1569A0 */

/* fzgx:begin fn_1_157920 */
extern u32 lbl_1_bss_8F578;

// fn_1_157920: simple getter returning global variable value
u32 fn_1_157920(void) {
    return lbl_1_bss_8F578;
}
/* fzgx:end fn_1_157920 */

/* fzgx:begin fn_1_1586A8 */
// fn_1_1586A8: empty in retail (single blr).
void fn_1_1586A8(void) {
}
/* fzgx:end fn_1_1586A8 */

/* fzgx:begin fn_1_158980 */
// fn_1_158980: empty in retail (single blr).
void fn_1_158980(void) {
}
/* fzgx:end fn_1_158980 */

/* fzgx:begin fn_1_15903C */
// fn_1_15903C: empty in retail (single blr).
void fn_1_15903C(void) {
}
/* fzgx:end fn_1_15903C */

/* fzgx:begin fn_1_159060 */
// fn_1_159060: empty in retail (single blr).
void fn_1_159060(void) {
}
/* fzgx:end fn_1_159060 */

/* fzgx:begin fn_1_159064 */
// fn_1_159064: returns a constant.
int fn_1_159064(void) {
    return 0;
}
/* fzgx:end fn_1_159064 */

/* fzgx:begin fn_1_1594AC */
extern char lbl_1_data_4C900[46];
extern int lbl_1_bss_8F588[188];

extern void OSReport(const char *format, ...);
extern void fn_1_465D0(void *object, int value);
extern void fn_80008BEC(void *dst, int value, int size);

typedef struct {
    void *object;
    s16 *indices;
    u8 data[0x20];
    s16 state;
    s16 state2;
    u8 type;
    u8 pad2d[0x0b];
    int value;
} Entry;

extern Entry lbl_1_data_4C810[4];

void fn_1_1594AC(int index, int flag) {
    Entry *entry;
    s16 value;
    int offset;

    entry = &lbl_1_data_4C810[index];
    if (entry->type >= 0x10) {
        OSReport(lbl_1_data_4C900);
    }

    if (flag != 0) {
        fn_1_465D0(entry->object, 6);
    } else {
        fn_1_465D0(entry->object, 5);
    }

    offset = 0;
    for (;;) {
        value = *(s16 *)((u8 *)entry->indices + offset);
        if (value == -1) {
            break;
        }
        lbl_1_bss_8F588[value] = index;
        offset += 2;
    }

    fn_80008BEC(entry->data, 0, 0x20);
    entry->state = 1;
    entry->state2 = 0;
    entry->value = flag;
}
/* fzgx:end fn_1_1594AC */

/* fzgx:begin fn_1_15B3E8 */
extern u8 lbl_1_bss_8F878[88];

u8 fn_1_15B3E8(void) {
    return lbl_1_bss_8F878[0x3a];
}
/* fzgx:end fn_1_15B3E8 */

/* fzgx:begin fn_1_15B408 */
extern u8 lbl_1_bss_8F878[88];

u8 fn_1_15B408(void) {
    return lbl_1_bss_8F878[0x3c];
}
/* fzgx:end fn_1_15B408 */

/* fzgx:begin fn_1_15B428 */
// fn_1_15B428: empty in retail (single blr).
void fn_1_15B428(void) {
}
/* fzgx:end fn_1_15B428 */

/* fzgx:begin fn_1_15B4F8 */
// fn_1_15B4F8: empty in retail (single blr).
void fn_1_15B4F8(void) {
}
/* fzgx:end fn_1_15B4F8 */

/* fzgx:begin fn_1_15B51C */
// fn_1_15B51C: empty in retail (single blr).
void fn_1_15B51C(void) {
}
/* fzgx:end fn_1_15B51C */

/* fzgx:begin fn_1_15B540 */
// fn_1_15B540: empty in retail (single blr).
void fn_1_15B540(void) {
}
/* fzgx:end fn_1_15B540 */

/* fzgx:begin fn_1_15B544 */
typedef struct {
    u32 flags;
    u8 pad_04[0x3c];
} RankingEntry;

// Set the high flag on each ranking entry managed by the singleton.
void fn_1_15B544(void) {
    s16 i;
    RankingEntry *entry;

    i = 0;
    entry = (RankingEntry *)lbl_1_bss_3BE0->unk_54;
    while (i < (s32)lbl_1_bss_3BE0->unk_48) {
        entry->flags |= 0x80000000u;
        i++;
        entry = (RankingEntry *)((u8 *)entry + 0x40);
    }
}
/* fzgx:end fn_1_15B544 */

/* fzgx:begin fn_1_15B588 */
// Clears the high bit of each ranking entry flag.
void fn_1_15B588(void) {
    u8 *entry;
    s16 i;
    Obj_1_data_2A7E0_At3C *obj;

    obj = lbl_1_data_2A7E0.unk_3C;
    entry = (u8 *)obj;
    i = 0;
    while (i < obj->unk_5E) {
        u32 *value = *(u32 **)(entry + 0xc4);

        entry += 4;
        i++;
        *value &= 0x7fffffff;
    }
}
/* fzgx:end fn_1_15B588 */
