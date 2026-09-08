#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_F56F8 */
extern f32 lbl_1_rodata_6D70;
extern f64 fn_80088538(f32);
extern f64 fn_800883E8(f32);

void fn_1_F56F8(f32 *out, f32 *in) {
    f32 value;
    f32 scale;
    f32 zero;

    value = fn_80088538(in[3]);
    scale = fn_800883E8(value);
    zero = lbl_1_rodata_6D70;
    out[3] = zero;
    if (scale > zero) {
        out[0] = value * in[0] / scale;
        out[1] = value * in[1] / scale;
        out[2] = value * in[2] / scale;
    } else {
        out[2] = zero;
        out[1] = zero;
        out[0] = zero;
    }
}
/* fzgx:end fn_1_F56F8 */

/* fzgx:begin fn_1_F5AAC */
// fn_1_F5AAC: empty in retail (single blr).
void fn_1_F5AAC(void) {
}
/* fzgx:end fn_1_F5AAC */

/* fzgx:begin fn_1_F5AEC */
// fn_1_F5AEC: empty in retail (single blr).
void fn_1_F5AEC(void) {
}
/* fzgx:end fn_1_F5AEC */

/* fzgx:begin fn_1_F5B38 */
// fn_1_F5B38: empty in retail (single blr).
void fn_1_F5B38(void) {
}
/* fzgx:end fn_1_F5B38 */

/* fzgx:begin fn_1_F5B84 */
// fn_1_F5B84: empty in retail (single blr).
void fn_1_F5B84(void) {
}
/* fzgx:end fn_1_F5B84 */

/* fzgx:begin fn_1_F70C0 */
// fn_1_F70C0: returns a constant.
int fn_1_F70C0(void) {
    return 1;
}
/* fzgx:end fn_1_F70C0 */

/* fzgx:begin fn_1_F70C8 */
// fn_1_F70C8: returns a constant.
int fn_1_F70C8(void) {
    return 1;
}
/* fzgx:end fn_1_F70C8 */

/* fzgx:begin fn_1_F7308 */
extern void fn_1_F7578(void);
extern u8 lbl_1_bss_7F028;

void fn_1_F7308(void) {
    fn_1_F7578();
    lbl_1_bss_7F028 &= 0xFE;
}
/* fzgx:end fn_1_F7308 */

/* fzgx:begin fn_1_F7578 */
extern u8 lbl_1_bss_7F02C[16];
extern u16 lbl_1_bss_7F03C[8];
extern void fn_8001D32C(u32 *);
extern void fn_80008BEC(void *, int, u32);

void fn_1_F7578(void) {
    u32 values[4];

    values[0] = 2;
    values[1] = 2;
    values[2] = 2;
    values[3] = 2;
    fn_8001D32C(values);
    fn_80008BEC(lbl_1_bss_7F02C, 0, 0x10);
    fn_80008BEC(lbl_1_bss_7F03C, 0, 0x10);
}
/* fzgx:end fn_1_F7578 */

/* fzgx:begin fn_1_F79C8 */
extern u8 lbl_1_bss_718E0[23016];
extern u16 lbl_1_data_414[36];
extern void fn_1_F8918(u8 *, Obj_1_bss_7F0C0 *, u8 *);

// Initialize the spline workspace and publish its active data region.
void fn_1_F79C8(void) {
    fn_1_F8918(&lbl_1_bss_718E0[0x1c0], &lbl_1_bss_7F0C0, lbl_1_bss_718E0);
    lbl_1_bss_84420 = (u32)((u8 *)&lbl_1_bss_7F0C0 + 0x4b5c);
    lbl_1_data_414[0] = 0xffff;
}
/* fzgx:end fn_1_F79C8 */

/* fzgx:begin fn_1_F8030 noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8030(void) {
    lbl_1_bss_7F0C0[19] = 0;
    lbl_1_bss_7F0C0[18] = 0;
}
/* fzgx:end fn_1_F8030 */

/* fzgx:begin fn_1_F8048 */
extern int fn_1_14D6D8(s16 value);
extern void fn_1_F8214(s16 value);

void fn_1_F8048(void) {
    s16 i;

    for (i = 0; i < 75; i++) {
        if (fn_1_14D6D8(i) & 0x40000000) {
            fn_1_F8214(i);
        }
    }
}
/* fzgx:end fn_1_F8048 */

/* fzgx:begin fn_1_F82E0 */
extern int fn_1_14D6D8(s16 value);
extern void fn_1_F83F0(s16 value);

void fn_1_F82E0(void) {
    s16 i;

    for (i = 0; i < 75; i++) {
        if (fn_1_14D6D8(i) & 0x20000000) {
            fn_1_F83F0(i);
        }
    }
}
/* fzgx:end fn_1_F82E0 */

/* fzgx:begin fn_1_F8A7C noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8A7C(void) {
    lbl_1_bss_7F0C0[4673] = 0;
}
/* fzgx:end fn_1_F8A7C */

/* fzgx:begin fn_1_F8AD4 noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8AD4(u32 value) {
    lbl_1_bss_7F0C0[4672] |= (0x8000u << 16) >> ((value & 0xff) + 0x10);
}
/* fzgx:end fn_1_F8AD4 */

/* fzgx:begin fn_1_F8B50 noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8B50(void) {
    lbl_1_bss_7F0C0[4676] = 1;
}
/* fzgx:end fn_1_F8B50 */

/* fzgx:begin fn_1_F8BA8 */
extern void fn_1_F8CCC(u32 value);

void fn_1_F8BA8(void) {
    fn_1_F8CCC(0x80000000u);
    fn_1_F8CCC(0x40000000u);
}
/* fzgx:end fn_1_F8BA8 */

/* fzgx:begin fn_1_F8C74 noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8C74(s32 value) {
    value *= 2;
    lbl_1_bss_7F0C0[4676] |= (((u32)1 << 31) >> (31 - (value + 1)));
}
/* fzgx:end fn_1_F8C74 */

/* fzgx:begin fn_1_F8CA0 noprologue */
#include "types.h"
extern u32 lbl_1_bss_7F0C0[5336];

void fn_1_F8CA0(s32 value) {
    value *= 2;
    lbl_1_bss_7F0C0[4676] &= ~(((u32)1 << 31) >> (31 - (value + 1)));
}
/* fzgx:end fn_1_F8CA0 */

/* fzgx:begin fn_1_F9D90 noprologue */
#include "types.h"
extern u16 lbl_1_bss_7F0C0[5336];

void fn_1_F9D90(void) {
    lbl_1_bss_7F0C0[0x34] = 0;
}
/* fzgx:end fn_1_F9D90 */

/* fzgx:begin fn_1_F9DA4 */
extern void fn_1_F9E38(void);

void fn_1_F9DA4(void) {
    fn_1_F9E38();
}
/* fzgx:end fn_1_F9DA4 */

/* fzgx:begin fn_1_F9E10 noprologue */
#include "types.h"
extern u16 lbl_1_bss_7F0C0[5336];

// fn_1_F9E10: Clear bit from u16 at offset 0x6a based on shift amount in lower byte.
void fn_1_F9E10(u32 param) {
    u16 *ptr = (u16*)((u8*)lbl_1_bss_7F0C0 + 0x6a);
    u32 shift_bits = param & 0x000000FF;
    s32 mask = 0x8000 >> shift_bits;
    u32 value = *ptr;
    *ptr = value & ~mask;
}
/* fzgx:end fn_1_F9E10 */

/* fzgx:begin fn_1_F9FEC */
// Returns the low flag bit stored in the spline state.
u32 fn_1_F9FEC(void) {
    u32 ptr = lbl_1_bss_84420;
    return (*(u8 *)(ptr + 0x1)) & 1;
}
/* fzgx:end fn_1_F9FEC */

/* fzgx:begin fn_1_FA004 */
// Reads the second status byte and returns its bit-mask value.
u32 fn_1_FA004(void) {
    u32 ptr = lbl_1_bss_84420;
    return (*(u8 *)(ptr + 0x1)) & 2;
}
/* fzgx:end fn_1_FA004 */

/* fzgx:begin fn_1_FA070 */
u32 fn_1_FA070(void) {
    u32 ptr = lbl_1_bss_84420;
    // Test the object's low flag bit.
    return (*(u8 *)(ptr + 2)) & 1;
}
/* fzgx:end fn_1_FA070 */

/* fzgx:begin fn_1_FA4C0 */
// Return the byte at offset 0x8 from the spline object referenced by the global.
u8 fn_1_FA4C0(void) {
    return *(u8 *)(lbl_1_bss_84420 + 0x8);
}
/* fzgx:end fn_1_FA4C0 */

/* fzgx:begin fn_1_FA69C noprologue */
#include "types.h"
extern u8 *lbl_1_bss_84420;

u8 fn_1_FA69C(s32 index) {
    if (index >= 0x29) {
        return 0;
    }
    return lbl_1_bss_84420[index + 0x5c];
}
/* fzgx:end fn_1_FA69C */

/* fzgx:begin fn_1_FA82C */
// fn_1_FA82C: empty in retail (single blr).
void fn_1_FA82C(void) {
}
/* fzgx:end fn_1_FA82C */

/* fzgx:begin fn_1_FA854 */
extern void fn_1_76BF8(void);
extern void fn_1_72648(void);

void fn_1_FA854(void) {
    fn_1_76BF8();
    fn_1_72648();
}
/* fzgx:end fn_1_FA854 */

/* fzgx:begin fn_1_FA898 */
// fn_1_FA898: empty in retail (single blr).
void fn_1_FA898(void) {
}
/* fzgx:end fn_1_FA898 */

/* fzgx:begin fn_1_FB180 */
void fn_1_FB180(int *value) {
    *value = 1;
}
/* fzgx:end fn_1_FB180 */
