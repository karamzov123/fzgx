#include "types.h"

/* fzgx:begin fn_1_D3214 */
#include "rel/main_rel/globals.h"
#include "rel/main_rel/reset.h"

extern s32 fn_1_B7E48(void);
extern void fn_8001AF64(void);
extern void fn_8001BDF0(s32);
extern void fn_8006B224(void);
extern void fn_8006FFCC(s32);
extern void fn_8006FDEC(void);
extern void fn_8006FEFC(void);
extern void fn_8000EE50(u32, u32);
extern void fn_1_C37A0(void);
extern void fn_1_F7578(void);
extern void fn_1_A0AA4(void);
extern void fn_1_A02F0(void);
extern void OSResetSystem(s32, u32, u32);
extern void fn_8000C49C(const char *, ...);
extern void *memset(u32, s32, u32);
extern void *memcpy(u32, u32, u32);
extern u8 lbl_801A66B0[4];

// Performs the pending reset after the reset request has been accepted.
void fn_1_D3214(void) {
    Obj_1_bss_7AD48 *state = &lbl_1_bss_7AD48;

    if (state->unk_0 != 0 || state->unk_1 != 0) {
        if (fn_1_B7E48() == 0 && state->unk_2 == 0) {
            state->unk_2 = 1;
            fn_8001AF64();
            fn_8001BDF0(1);
            if (state->unk_4 == 0 || ((s32 (*)(void))state->unk_4)() != 0) {
                fn_1_C37A0();
                fn_1_F7578();
                fn_8006B224();
                fn_1_A0AA4();
                fn_1_A02F0();
                fn_8006FFCC(0);
                fn_8006FDEC();
                fn_8006FEFC();
                fn_8000EE50(0x80700000, 0x80700020); // fzgx-allow: A1 fixed MEM1 scratch address
                memset(0x80700000, 0, 0x20); // fzgx-allow: A1 fixed MEM1 scratch address
                memcpy(0x80700000, (u32)lbl_801A66B0, 4); // fzgx-allow: A1 fixed MEM1 scratch address
                state->unk_0 = 0;
                OSResetSystem(0, state->unk_8, 0);
                fn_8000C49C((const char *)&lbl_1_data_3DBC0, 0x76,
                            (const char *)lbl_1_data_3DBC8);
            }
        }
    }
}
/* fzgx:end fn_1_D3214 */

/* fzgx:begin fn_1_D332C */
#include "rel/main_rel/reset.h"

extern const f64 lbl_1_rodata_6160;
extern const f64 lbl_1_rodata_6158;
extern Obj_1_bss_7AD48 lbl_1_bss_7AD48;

extern int fn_8000F5F0(void);
extern int fn_1_A6F2C(void);
extern int fn_80006AF4(void);
extern int fn_1_A6F6C(void);
extern int fn_1_A6FA0(void);

void fn_1_D332C(u16 *data) {
    u16 *entry;
    Obj_1_bss_7AD48 *state = &lbl_1_bss_7AD48;
    int i;

    if (state->unk_9 == 1 && fn_8000F5F0() == 0) {
        state->unk_A = 0xff;
        state->unk_8 = 2;
    } else {
        state->unk_8 = 1;
    }

    if (fn_8000F5F0() != 0) {
        state->unk_9 = 1;
    }

    if ((f64)state->unk_A > lbl_1_rodata_6158) {
        state->unk_0 = 1;
    }

    entry = data;
    for (i = 0; i < 4; i++) {
        if ((*entry & 0x1600) == 0x1600) {
            state->unk_A++;
            break;
        }
        entry += 6;
    }

    if (i == 4) {
        state->unk_A = 0;
    }

    if (state->unk_0 != 0 &&
        fn_1_A6F2C() != 0 &&
        fn_80006AF4() == 0) {
        state->unk_0 = 0;
        state->unk_9 = 0;
        state->unk_A = 0;
        if (fn_1_A6F6C() == 0) {
            fn_1_A6FA0();
        }
    }
}
/* fzgx:end fn_1_D332C */

/* fzgx:begin fn_1_D3474 */
#include "rel/main_rel/reset.h"

extern void fn_8001AF64(void);
extern void fn_8001BDF0(int arg0);
extern void fn_8006B224(void);
extern void fn_8006FFCC(int arg0);
extern void fn_8006FDEC(void);
extern void fn_8006FEFC(void);
extern void fn_8000EE50(void *arg0, void *arg1);
extern int fn_8000C49C(void *arg0, int arg1, ...);
extern void *memset(void *dest, int value, unsigned long size);
extern void *memcpy(void *dest, const void *src, unsigned long size);
extern void OSResetSystem(int reset, u32 reset_code, int force);

extern u8 lbl_801A66B0[4];

#define RESET_AREA ((u8 *)((u32)0x8070 << 16))

void fn_1_D3474(void) {
    Obj_1_bss_7AD48 *state = &lbl_1_bss_7AD48;
    int result;
    u32 callback;

    state->unk_0 = 1;
    result = fn_1_B7E48();
    if (result == 0 && state->unk_2 == 0) {
        state->unk_2 = 1;
        fn_8001AF64();
        fn_8001BDF0(1);
        callback = state->unk_4;
        if (callback != 0) {
            result = ((int (*)(void))callback)();
            if (result == 0) {
                return;
            }
        }
        fn_1_C37A0();
        fn_1_F7578();
        fn_8006B224();
        fn_1_A0AA4();
        fn_1_A02F0();
        fn_8006FFCC(0);
        fn_8006FDEC();
        fn_8006FEFC();
        fn_8000EE50(RESET_AREA, RESET_AREA + 0x20);
        memset(RESET_AREA, 0, 0x20);
        memcpy(RESET_AREA, lbl_801A66B0, 4);
        state->unk_0 = 0;
        OSResetSystem(0, state->unk_8, 0);
        fn_8000C49C(&lbl_1_data_3DBC0, 0x76, lbl_1_data_3DBC8);
    }
}
/* fzgx:end fn_1_D3474 */

/* fzgx:begin fn_1_D357C */
#include "rel/main_rel/reset.h"

u8 fn_1_D357C(void) {
    return lbl_1_bss_7AD48.unk_0;
}
/* fzgx:end fn_1_D357C */

/* fzgx:begin fn_1_D358C */
extern void fn_80070F44(void);
extern void fn_1_D3B6C(void);

void fn_1_D358C(void) {
    fn_80070F44();
    fn_1_D3B6C();
}
/* fzgx:end fn_1_D358C */

/* fzgx:begin fn_1_D35B0 */
extern void fn_80071100(void);
extern void fn_1_D3B6C(void);

// Wrapper that calls fn_80071100 followed by fn_1_D3B6C.
void fn_1_D35B0(void) {
    fn_80071100();
    fn_1_D3B6C();
}
/* fzgx:end fn_1_D35B0 */
