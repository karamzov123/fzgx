#include "types.h"

/* fzgx:begin fn_1_15BE38 */
typedef struct {
    u32 count;
    void **items;
} ItemList;

extern u32 lbl_1_bss_8FD60[2];
extern int fn_1_15BCDC(void *);

u32 fn_1_15BE38(void) {
    u32 offset;
    u32 index;

    offset = 0;
    index = 0;
    while (index < (*(ItemList **)&lbl_1_bss_8FD60)->count) {
        if (fn_1_15BCDC((*(ItemList **)&lbl_1_bss_8FD60)->items[offset >> 2]) != 0) {
            return index & 0xffff;
        }
        offset += 4;
        index++;
    }
    return 0xffff;
}
/* fzgx:end fn_1_15BE38 */

/* fzgx:begin fn_1_15C35C */
s32 fn_1_15C35C(s32 a, s32 b, s32 c, s32 d) {
    return a * b - c + d;
}
/* fzgx:end fn_1_15C35C */

/* fzgx:begin fn_1_15E1E8 */
extern void fn_1_4060(void);

void fn_1_15E1E8(u8 *value) {
    u8 state = *value;

    if (state == 0) {
        fn_1_4060();
    } else {
        *value = state - 1;
    }
}
/* fzgx:end fn_1_15E1E8 */

/* fzgx:begin fn_1_15E220 */
extern void fn_1_4060(void);

void fn_1_15E220(u8 *value) {
    u8 state = *value;

    if (state == 0) {
        fn_1_4060();
    } else if (state != 0xff) {
        *value = state - 1;
    }
}
/* fzgx:end fn_1_15E220 */

/* fzgx:begin fn_1_15E260 */
#include "rel/main_rel/bg_win.h"

// Marks the indexed background-window entry as active.
void fn_1_15E260(s32 index) {
    (&lbl_1_bss_8FDA8.unk_0)[index * 0x34] |= 4;
}
/* fzgx:end fn_1_15E260 */

/* fzgx:begin fn_1_15E3E0 */
typedef struct {
    u8 pad[0x13f4];
    u8 unk_13F4;
} Obj_1_bss_3C30_local;

typedef struct {
    u8 pad[0xc];
    u32 unk_C;
    u8 tail[0x24];
} Obj_1_bss_8FDA8_local;

extern Obj_1_bss_3C30_local lbl_1_bss_3C30;
extern Obj_1_bss_8FDA8_local lbl_1_bss_8FDA8[1];
extern u32 lbl_1_bss_8FE80[8];

void fn_1_15E3E0(s32 index, u32 value) {
    s32 slot = (lbl_1_bss_3C30.unk_13F4 - 1) % 4;

    lbl_1_bss_8FE80[slot] = index;
    if (index != -1) {
        lbl_1_bss_8FDA8[index].unk_C = value;
    }
}
/* fzgx:end fn_1_15E3E0 */

/* fzgx:begin fn_1_15E540 */
#include "rel/main_rel/globals.h"

typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 pad_2[0x2e];
    f32 unk_30;
} WinEntry;

extern void *fn_1_435C(void *);
extern unsigned char lbl_1_data_4C994[12];
extern void fn_1_15E220(u8 *);
extern void fn_1_3F8C(void *, void *, u8 *, s32);
extern WinEntry lbl_1_bss_8FDA8[];

// Initializes a window entry once and marks it ready for reuse.
void fn_1_15E540(s32 index, void *arg) {
    if (!(lbl_1_bss_8FDA8[index].unk_0 & 1)) {
        void *value = fn_1_435C(arg);
        fn_1_3F8C(lbl_1_data_4C994, fn_1_15E220,
                  &lbl_1_bss_8FDA8[index].unk_1, 13);
        lbl_1_bss_8FDA8[index].unk_1 = 0xff;
        fn_1_435C(value);
        lbl_1_bss_8FDA8[index].unk_0 |= 1;
    }
}
/* fzgx:end fn_1_15E540 */

/* fzgx:begin fn_1_15F618 */
#include "rel/main_rel/bg_win.h"

void *fn_1_15F618(s32 index) {
    return (u8 *)&lbl_1_bss_8FDA8 + index * 0x34;
}
/* fzgx:end fn_1_15F618 */
