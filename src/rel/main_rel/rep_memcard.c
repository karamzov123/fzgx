#include "types.h"

/* fzgx:begin fn_1_154CC4 */
#include "rel/main_rel/rep_memcard.h"

void fn_1_154CC4(u32 arg0) {
    lbl_1_data_49AB4.unk_0 = arg0;
}
/* fzgx:end fn_1_154CC4 */

/* fzgx:begin fn_1_154CD0 noprologue */
#include "types.h"

extern u32 lbl_1_bss_8ED90;
typedef struct {
    u32 unk_0;  // 2 loads, 3 stores
    u8 pad_4[0x8];
} Obj_1_bss_8ED94;
extern Obj_1_bss_8ED94 lbl_1_bss_8ED94;
extern u8 lbl_1_data_49AC8[0xE];

extern u32 lbl_801A6410;

extern void fn_80008BA8(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_46B4(u32 arg0, u32 arg1, u8 *arg2, s32 arg3);
extern u32 fn_1_4630(u32 arg0, u32 arg1, u8 *arg2, s32 arg3);

void fn_1_154CD0(u32 arg0, u32 arg1) {
    if (lbl_1_bss_8ED90 != 0) {
        fn_1_46B4(lbl_801A6410, lbl_1_bss_8ED90, lbl_1_data_49AC8, 0x12f);
        lbl_1_bss_8ED90 = 0;
        lbl_1_bss_8ED94.unk_0 = 0;
    }

    lbl_1_bss_8ED90 = fn_1_4630(lbl_801A6410, arg1, lbl_1_data_49AC8, 0x133);
    fn_80008BA8(lbl_1_bss_8ED90, arg0, arg1);
    lbl_1_bss_8ED94.unk_0 = arg1;
}
/* fzgx:end fn_1_154CD0 */

/* fzgx:begin fn_1_154D84 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/rep_memcard.h"

extern u32 lbl_801A6410;
extern u8 lbl_1_data_49AC8[14];
extern void fn_80008BA8(u32 arg0, u32 arg1, u32 arg2);
extern void fn_1_46B4(u32 arg0, u32 arg1, u8 *arg2, s32 arg3);

// Saves the pending memory-card data and clears the pending state.
u32 fn_1_154D84(u32 arg0) {
    if (lbl_1_bss_8ED90 == 0) {
        return 0;
    }
    if (arg0 == 0) {
        return 0;
    }

    fn_80008BA8(arg0, lbl_1_bss_8ED90, lbl_1_bss_8ED94.unk_0);
    fn_1_46B4(lbl_801A6410, lbl_1_bss_8ED90, lbl_1_data_49AC8, 0x13f);
    lbl_1_bss_8ED90 = 0;
    lbl_1_bss_8ED94.unk_0 = 0;
    return 1;
}
/* fzgx:end fn_1_154D84 */

/* fzgx:begin fn_1_154E24 */
extern u8 lbl_1_bss_8EDA0;

u8 fn_1_154E24(void) {
    return lbl_1_bss_8EDA0;
}
/* fzgx:end fn_1_154E24 */

/* fzgx:begin fn_1_154E34 */
#include "rel/main_rel/rep_memcard.h"

u32 fn_1_154E34(u32 *arg0) {
    *arg0 = lbl_1_bss_8ED94.unk_0;
    return lbl_1_bss_8ED90;
}
/* fzgx:end fn_1_154E34 */

/* fzgx:begin fn_1_154ED0 */
#include "rel/main_rel/rep_memcard.h"

extern void fn_1_154F1C(void);

typedef struct {
    u8 pad_0[0x134];
    Obj_1_bss_8EDF0_At10 *unk_134;
} Fn_1_154ED0_Arg;

void fn_1_154ED0(Fn_1_154ED0_Arg *arg0) {
    fn_1_154F1C();
    lbl_1_bss_8EDF0.unk_C = (u32)arg0;
    lbl_1_bss_8EDF0.unk_10 = arg0->unk_134;
    lbl_1_bss_8EDF0.unk_0 = 0;
    lbl_1_bss_8EDF0.unk_4 = 0;
}
/* fzgx:end fn_1_154ED0 */

/* fzgx:begin fn_1_154F1C */
#include "rel/main_rel/rep_memcard.h"

extern void *memset(void *dest, int value, u32 size);

// Reset the memory-card state and clear its backing buffer.
void fn_1_154F1C(void) {
    lbl_1_bss_8EDF0.unk_1C = 0;
    lbl_1_bss_8EDF0.unk_14 = 0;
    lbl_1_bss_8EDF0.unk_18 = 0;
    memset(&lbl_1_bss_8EDF0.unk_34, 0, 250);
    lbl_1_bss_8EDF0.unk_34 = 0;
}
/* fzgx:end fn_1_154F1C */

/* fzgx:begin fn_1_154F74 */
#include "rel/main_rel/rep_memcard.h"

// Stores the two memory-card state values used by the replication subsystem.
void fn_1_154F74(u32 arg0, u32 arg1) {
    lbl_1_bss_8EDF0.unk_20 = arg0;
    lbl_1_bss_8EDF0.unk_24 = arg1;
}
/* fzgx:end fn_1_154F74 */

/* fzgx:begin fn_1_154F88 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/rep_memcard.h"

// Stores the current float value in the memory-card state.
void fn_1_154F88(f32 value) {
    lbl_1_bss_8EDF0.unk_2C = value;
}
/* fzgx:end fn_1_154F88 */

/* fzgx:begin fn_1_154F98 */
#include "rel/main_rel/rep_memcard.h"

void fn_1_154F98(u32 value) {
    lbl_1_bss_8EDF0.unk_30 = value;
}
/* fzgx:end fn_1_154F98 */

/* fzgx:begin fn_1_154FA8 */
#include "rel/main_rel/rep_memcard.h"

void fn_1_154FA8(void) {
    lbl_1_bss_8EDF0.unk_8 |= 4;
}
/* fzgx:end fn_1_154FA8 */

/* fzgx:begin fn_1_154FC0 */
#include "rel/main_rel/rep_memcard.h"

void fn_1_154FC0(void) {
    lbl_1_bss_8EDF0.unk_8 &= ~4;
}
/* fzgx:end fn_1_154FC0 */
