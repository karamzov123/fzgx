#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/custom_memcard.h"

extern u32 lbl_801A6410;
extern u8 fn_1_B7C00(void);
extern s32 fn_1_B7CD4(void);
extern s32 fn_1_B7C5C(void);
extern void OSReport(const char* format, ...);
extern void fn_1_46B4(u32 arg0, void* arg1, u8* arg2, s32 arg3);
extern void fn_80008BA8(void*, void*, s32);
extern u32 lbl_1_bss_8CA6C[1605];
extern void fn_1_1324C4(void *arg0);
extern void fn_1_132774(void *arg0);

/* fzgx:begin fn_1_1312F0 */
// Reset the memcard state after reporting an unavailable card.

void fn_1_1312F0(void) {
    u8* state = (u8*)&lbl_1_bss_8CA40;
    void* memcard;

    if (fn_1_B7C00() == 0) {
        if (fn_1_B7CD4() == 0) {
            OSReport((const char*)lbl_1_data_40EF8, fn_1_B7C5C());
        }

        memcard = *(void**)(state + 0x18);
        *(s32*)(state + 0x00) = -1;
        *(s32*)(state + 0x08) = -1;
        *(s32*)(state + 0x0c) = 0;
        if (memcard != 0) {
            fn_1_46B4(lbl_801A6410, memcard, lbl_1_data_40EA4, 0x136);
            *(void**)(state + 0x18) = 0;
        }
    }
}
/* fzgx:end fn_1_1312F0 */

/* fzgx:begin fn_1_1314A4 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/custom_memcard.h"

extern u32 lbl_801A6410;

extern u8 fn_1_B7C00(void);
extern s32 fn_1_B7CD4(void);
extern s32 fn_1_B7C5C(void);
extern void OSReport(u8* format, ...);
extern void fn_80008BA8(void*, void*, s32);
extern void fn_1_46B4(void*, void*, u8*, s32);

// Reset the card state and release the card after an unavailable-card report.
void fn_1_1314A4(void) {
    u8* state = (u8*)&lbl_1_bss_8CA40;
    void* card;

    if (fn_1_B7C00() == 0) {
        if (fn_1_B7CD4() == 0) {
            OSReport(lbl_1_data_40EF8, fn_1_B7C5C());
        }
        fn_80008BA8(*(void**)(state + 0x10), *(void**)(state + 0x18), 0x20700);
        card = *(void**)(state + 0x18);
        *(s32*)(state + 0x00) = -1;
        *(s32*)(state + 0x08) = -1;
        *(void**)(state + 0x10) = 0;
        if (card != 0) {
            fn_1_46B4((void*)lbl_801A6410, card, lbl_1_data_40EA4, 0x188);
            *(void**)(state + 0x18) = 0;
        }
    }
}
/* fzgx:end fn_1_1314A4 */

/* fzgx:begin fn_1_1317B4 */
u32 fn_1_1317B4(void) {
    return lbl_1_bss_8CA6C[0];
}
/* fzgx:end fn_1_1317B4 */

/* fzgx:begin fn_1_1318B8 */
u8 fn_1_1318B8(void) {
    return lbl_1_bss_8E380 != 0;
}
/* fzgx:end fn_1_1318B8 */

/* fzgx:begin fn_1_1318D4 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/custom_memcard.h"

extern u8 fn_1_B7C00(void);
extern s32 fn_1_B7CD4(void);
extern s32 fn_1_B7C5C(void);
extern u32 lbl_801A6410;
extern void OSReport(const u8* format, ...);
extern void fn_1_46B4(u32 arg0, u32 arg1, u8* arg2, u32 arg3);

// Completes the pending memory-card operation and clears its active flag.
s32 fn_1_1318D4(void) {
    s32 result;

    if (lbl_1_bss_8E380 == 0) {
        return 0;
    }

    if (fn_1_B7C00() != 0) {
        return 1;
    }

    if (fn_1_B7CD4() == 0) {
        result = fn_1_B7C5C();
        OSReport(lbl_1_data_40EF8, result);
    }

    if (lbl_1_bss_8E384.unk_0 != 0) {
        fn_1_46B4(lbl_801A6410, lbl_1_bss_8E384.unk_0, lbl_1_data_40EA4, 0x22b);
        lbl_1_bss_8E384.unk_0 = 0;
    }

    lbl_1_bss_8E380 = 0;
    return 1;
}
/* fzgx:end fn_1_1318D4 */

/* fzgx:begin fn_1_132488 */
void fn_1_132488(void *arg0) {
    if ((lbl_1_bss_8B3A0.unk_94 & 0x40000000) != 0) {
        fn_1_1324C4(arg0);
    } else {
        fn_1_132774(arg0);
    }
}
/* fzgx:end fn_1_132488 */

/* fzgx:begin fn_1_137364 */
// Writes the indexed three-byte value, or the fallback value when the index is out of range.
void fn_1_137364(s16 arg0, u8* arg1, u8* arg2, u8* arg3) {
    if (arg0 < 0x29) {
        u32* values = (u32*)&lbl_1_data_416B8;

        *arg1 = values[arg0 * 3];
        *arg2 = values[arg0 * 3 + 1];
        *arg3 = values[arg0 * 3 + 2];
    } else {
        *arg2 = 2;
        *arg1 = 2;
        *arg3 = 2;
    }
}
/* fzgx:end fn_1_137364 */

/* fzgx:begin fn_1_13F7F8 */
u32 fn_1_13F7F8(s16 arg0, s16 arg1) {
    return ((u32 (*)[6])&lbl_1_data_414D8)[arg0][arg1];
}
/* fzgx:end fn_1_13F7F8 */
