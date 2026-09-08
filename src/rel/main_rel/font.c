#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_4955C */
extern f32 lbl_1_data_1AEA8[616];

void fn_1_4955C(f32 value1, f32 value2) {
    s16 index = *(s16 *)&lbl_1_bss_4B9CC.unk_C;
    f32 scale = ((f32 *)lbl_1_data_1AEA8)[index * 14 + 13];

    // Scale both font dimensions using the active font's metrics.
    lbl_1_bss_4B9CC.unk_1C = value1 * scale;
    lbl_1_bss_4B9CC.unk_20 = value2 * scale;
}
/* fzgx:end fn_1_4955C */

/* fzgx:begin fn_1_495C8 */
void fn_1_495C8(u8 value) {
    // Update the current font state flag.
    lbl_1_bss_4B9CC.unk_36 = value;
}
/* fzgx:end fn_1_495C8 */

/* fzgx:begin fn_1_4966C */
void fn_1_4966C(f32 value1, f32 value2) {
    // Store the two font scale values used by the renderer.
    lbl_1_bss_4B9CC.unk_3C = value1;
    lbl_1_bss_4B9CC.unk_40 = value2;
}
/* fzgx:end fn_1_4966C */

/* fzgx:begin fn_1_496FC */
 // Store the two font coordinates in the shared state.
void fn_1_496FC(f32 value1, f32 value2) {
    lbl_1_bss_4B9CC.unk_4 = value1;
    lbl_1_bss_4B9CC.unk_0 = value1;
    lbl_1_bss_4B9CC.unk_8 = value2;
}
/* fzgx:end fn_1_496FC */

/* fzgx:begin fn_1_49738 */
 // Store the supplied value in the font state used by subsequent rendering.
void fn_1_49738(u32 value) {
    lbl_1_bss_4B9CC.unk_54 = value;
}
/* fzgx:end fn_1_49738 */

/* fzgx:begin fn_1_4AEB4 */
extern f32 lbl_1_bss_3E05C;

void fn_1_4AEB4(f32 value) {
    lbl_1_bss_3E05C = value;
}
/* fzgx:end fn_1_4AEB4 */

/* fzgx:begin fn_1_4B16C */
extern f32 fn_1_4B1D4(s32 mode, s32 value);

s32 fn_1_4B16C(s32 value) {
    return (s32)fn_1_4B1D4(2, value);
}
/* fzgx:end fn_1_4B16C */

/* fzgx:begin fn_1_4B1A0 */
extern f32 fn_1_4B1D4(s32 mode, s32 value);

s32 fn_1_4B1A0(s32 value) {
    return (s32)fn_1_4B1D4(3, value);
}
/* fzgx:end fn_1_4B1A0 */

/* fzgx:begin fn_1_4B8CC */
extern u32 lbl_1_bss_4BA5C;

void fn_1_4B8CC(void) {
    lbl_1_bss_4BA5C = 0;
}
/* fzgx:end fn_1_4B8CC */

/* fzgx:begin fn_1_4BD48 */
// fn_1_4BD48: empty in retail (single blr).
void fn_1_4BD48(void) {
}
/* fzgx:end fn_1_4BD48 */

/* fzgx:begin fn_1_4DCB8 noprologue */
#include "types.h"
extern u32 lbl_1_bss_4B9CC[23];

u32 fn_1_4DCB8(u32 value) {
    u32 old_value = lbl_1_bss_4B9CC[22];
    lbl_1_bss_4B9CC[22] = value;
    return old_value;
}
/* fzgx:end fn_1_4DCB8 */

/* fzgx:begin fn_1_4DCD0 */
// Returns the address of the font data embedded in the global object.
u8 *fn_1_4DCD0(void) {
    return (u8 *)&lbl_1_bss_4C688.unk_C;
}
/* fzgx:end fn_1_4DCD0 */

/* fzgx:begin fn_1_4DD50 */
u8 *fn_1_4DD50(void) {
    // Return the byte address used by the font data at offset 0x41.
    return &lbl_1_bss_4C688.pad_4[0x3d];
}
/* fzgx:end fn_1_4DD50 */

/* fzgx:begin fn_1_4DDA0 */
 // Return the byte address six bytes into the shared font state.
u8 *fn_1_4DDA0(void) {
    return &lbl_1_bss_4C688.pad_4[2];
}
/* fzgx:end fn_1_4DDA0 */

/* fzgx:begin fn_1_4DE44 noprologue */
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
/* fzgx:end fn_1_4DE44 */

/* fzgx:begin fn_1_4E6F4 */
extern void fn_1_A722C(void);
extern void fn_80074918(s32, s32, s32);

void fn_1_4E6F4(void) {
    fn_1_A722C();
    fn_80074918(1, 3, 1);
}
/* fzgx:end fn_1_4E6F4 */

/* fzgx:begin fn_1_4F724 */
extern u16 lbl_1_bss_646D2[7];

// fn_1_4F724: initialize lbl_1_bss_646D2[0] to zero.
void fn_1_4F724(void) {
    lbl_1_bss_646D2[0] = 0;
}
/* fzgx:end fn_1_4F724 */

/* fzgx:begin fn_1_51564 */
extern u16 lbl_1_bss_4C678[0xC033];

void fn_1_51564(u16 a, u16 b, u16 c, u16 d, u16 e, u16 f) {
    lbl_1_bss_4C678[0xC02e] = a;
    lbl_1_bss_4C678[0xC02f] = b;
    lbl_1_bss_4C678[0xC030] = c;
    lbl_1_bss_4C678[0xC031] = d;
    lbl_1_bss_4C678[0xC032] = e;
    lbl_1_bss_4C678[0xC033] = f;
}
/* fzgx:end fn_1_51564 */

/* fzgx:begin fn_1_51990 */
extern u32 lbl_1_bss_4C678[2];

// fn_1_51990: Copy a value to structure field at +0x4c, then set bit 0x01000000 in field at +0x30.
void fn_1_51990(void *obj) {
    *(u32 *)((u8 *)obj + 0x4c) = lbl_1_bss_4C678[0];
    *(u32 *)((u8 *)obj + 0x30) |= 0x01000000;
}
/* fzgx:end fn_1_51990 */

/* fzgx:begin fn_1_52088 */
extern u32 lbl_1_bss_4E6A8;
extern u32 lbl_1_bss_4E6AC;

void fn_1_52088(void) {
    lbl_1_bss_4E6A8 = 0;
    lbl_1_bss_4E6AC = 0;
}
/* fzgx:end fn_1_52088 */

/* fzgx:begin fn_1_520CC */
extern u32 lbl_1_bss_646E0;
extern u32 lbl_1_bss_646E4;
extern u32 lbl_1_bss_4E6A8;
extern u32 lbl_1_bss_4E6AC;

void fn_1_520CC(void) {
    u32 value_a;
    u32 value_b;

    value_a = lbl_1_bss_646E0;
    value_b = lbl_1_bss_646E4;
    lbl_1_bss_4E6A8 = value_a;
    lbl_1_bss_4E6AC = value_b;
}
/* fzgx:end fn_1_520CC */

/* fzgx:begin fn_1_52B68 */
extern void fn_1_52BF8(void *arg0, void *arg1, s32 arg2);

void fn_1_52B68(void *arg0, void *arg1) {
    fn_1_52BF8(arg0, arg1, 0);
}
/* fzgx:end fn_1_52B68 */

/* fzgx:begin fn_1_5417C */
extern u32 lbl_801A66B4;
extern void fn_1_541A8(void *arg, u32 value);

void fn_1_5417C(void *arg) {
    fn_1_541A8(arg, lbl_801A66B4);
}
/* fzgx:end fn_1_5417C */

/* fzgx:begin fn_1_541A8 */
extern u8 lbl_1_data_1C5DC[4];
extern u8 lbl_1_data_1C5EC[100];

void *fn_1_541A8(void *arg, s32 value) {
    s32 special;
    u32 remainder;
    s32 index;
    u32 *row;

    if (arg == 0) {
        return lbl_1_data_1C5DC;
    }

    remainder = (u32)arg - ((u32)arg / 100) * 100;
    special = 0;
    if (remainder >= 10 && remainder <= 20) {
        special = 1;
    }

    if (special != 0) {
        index = 3;
    } else {
        arg = (void *)(remainder - (remainder / 10) * 10);
        special = 0;
        if ((u32)arg >= 4 || arg == 0) {
            special = 1;
        }
        index = 3;
        if (special == 0) {
            index = (u32)arg - 1;
        }
    }

    row = (u32 *)(lbl_1_data_1C5EC + index * 24);
    return (void *)row[value < 0 ? 0 : (value > 5 ? 5 : value)];
}
/* fzgx:end fn_1_541A8 */

/* fzgx:begin fn_1_54298 */
extern u32 lbl_1_bss_4C680;

u32 fn_1_54298(void) {
    return lbl_1_bss_4C680;
}
/* fzgx:end fn_1_54298 */

/* fzgx:begin fn_1_542A8 */
extern u32 lbl_1_bss_4C684;

u32 fn_1_542A8(void) {
    return lbl_1_bss_4C684;
}
/* fzgx:end fn_1_542A8 */

/* fzgx:begin fn_1_547F8 */
typedef struct fn_1_547F8_node {
    struct fn_1_547F8_node *next;
    void (*callback)(struct fn_1_547F8_node *);
} fn_1_547F8_node;

void fn_1_547F8(fn_1_547F8_node *node) {
    while (node != 0) {
        if (node->callback != 0) {
            node->callback(node);
        }
        node = node->next;
    }
}
/* fzgx:end fn_1_547F8 */

/* fzgx:begin fn_1_550A8 */
extern u8 lbl_1_bss_6C710[136];
extern void *lbl_801A66CC;
extern void fn_800794F0(u8 *data, void *value, s32 size);

void fn_1_550A8(void) {
    fn_800794F0(lbl_1_bss_6C710, (u8 *)lbl_801A66CC + 0x50, 0x84);
}
/* fzgx:end fn_1_550A8 */

/* fzgx:begin fn_1_550E0 */
extern u8 lbl_1_bss_6C710[136];
extern void *lbl_801A66CC;
extern void fn_800794F0(u8 *data, void *value, s32 size);

void fn_1_550E0(void) {
    fn_800794F0((u8 *)lbl_801A66CC + 0x50, lbl_1_bss_6C710, 0x84);
}
/* fzgx:end fn_1_550E0 */

/* fzgx:begin fn_1_556B8 noprologue */
#include "types.h"
extern u8 lbl_1_bss_6C7A4[40];
extern void fn_1_563E4(u8 *data);
extern void fn_80077F8C(void *value);
extern void fn_1_56530(void);

void fn_1_556B8(void *value) {
    fn_1_563E4(lbl_1_bss_6C7A4);
    fn_80077F8C(value);
    fn_1_56530();
}
/* fzgx:end fn_1_556B8 */

/* fzgx:begin fn_1_556F8 */
extern f32 lbl_1_bss_6C7A0;
extern s32 fn_1_54F5C(u8 *data, f32 value, f32 scale);
extern void fn_1_563E4(void *data);
extern void fn_80077F8C(void *value);
extern void fn_1_56530(void);

typedef struct {
    u8 unk_00[8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    f32 unk_14;
} Fn1_556F8Object;

/* Finishes the current font operation after testing its scale threshold. */
void fn_1_556F8(Fn1_556F8Object *value) {
    if (fn_1_54F5C((u8 *)&value->unk_08, value->unk_14, lbl_1_bss_6C7A0)) {
        fn_1_563E4(&lbl_1_bss_6C7A4);
        fn_80077F8C(value);
        fn_1_56530();
        return;
    }

    fn_1_56530();
}
/* fzgx:end fn_1_556F8 */

/* fzgx:begin fn_1_55FD0 */
extern void fn_80074CF4(void);

void fn_1_55FD0(void) {
    fn_80074CF4();
}
/* fzgx:end fn_1_55FD0 */

/* fzgx:begin fn_1_5638C */
extern void fn_80074A7C(void);

// Stores the current value for the font subsystem and refreshes its state.
void fn_1_5638C(u32 value) {
    lbl_1_bss_6C7A4.unk_20 = value;
    fn_80074A7C();
}
/* fzgx:end fn_1_5638C */

/* fzgx:begin fn_1_565F4 */
// fn_1_565F4: empty in retail (single blr).
void fn_1_565F4(void) {
}
/* fzgx:end fn_1_565F4 */
