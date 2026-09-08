#include "types.h"

/* fzgx:begin fn_1_EC900 */
extern u32 lbl_1_bss_7B1A4[2];

u32 fn_1_EC900(void) {
    return lbl_1_bss_7B1A4[0];
}
/* fzgx:end fn_1_EC900 */

/* fzgx:begin fn_1_ECDFC */
// fn_1_ECDFC: empty in retail (single blr).
void fn_1_ECDFC(void) {
}
/* fzgx:end fn_1_ECDFC */

/* fzgx:begin fn_1_ECE00 */
// fn_1_ECE00: empty in retail (single blr).
void fn_1_ECE00(void) {
}
/* fzgx:end fn_1_ECE00 */

/* fzgx:begin fn_1_ECF68 */
extern void fn_1_F23E8(void);

void fn_1_ECF68(void) {
    fn_1_F23E8();
}
/* fzgx:end fn_1_ECF68 */

/* fzgx:begin fn_1_EF0A0 */
// fn_1_EF0A0: empty in retail (single blr).
void fn_1_EF0A0(void) {
}
/* fzgx:end fn_1_EF0A0 */

/* fzgx:begin fn_1_EF0A4 */
// fn_1_EF0A4: empty in retail (single blr).
void fn_1_EF0A4(void) {
}
/* fzgx:end fn_1_EF0A4 */

/* fzgx:begin fn_1_EF484 */
// fn_1_EF484: empty in retail (single blr).
void fn_1_EF484(void) {
}
/* fzgx:end fn_1_EF484 */

/* fzgx:begin fn_1_EF4F4 */
// fn_1_EF4F4: empty in retail (single blr).
void fn_1_EF4F4(void) {
}
/* fzgx:end fn_1_EF4F4 */

/* fzgx:begin fn_1_EF5DC */
// fn_1_EF5DC: empty in retail (single blr).
void fn_1_EF5DC(void) {
}
/* fzgx:end fn_1_EF5DC */

/* fzgx:begin fn_1_EF764 */
// fn_1_EF764: empty in retail (single blr).
void fn_1_EF764(void) {
}
/* fzgx:end fn_1_EF764 */

/* fzgx:begin fn_1_EF85C */
// fn_1_EF85C: empty in retail (single blr).
void fn_1_EF85C(void) {
}
/* fzgx:end fn_1_EF85C */

/* fzgx:begin fn_1_EF920 */
// fn_1_EF920: empty in retail (single blr).
void fn_1_EF920(void) {
}
/* fzgx:end fn_1_EF920 */

/* fzgx:begin fn_1_EF978 */
// fn_1_EF978: empty in retail (single blr).
void fn_1_EF978(void) {
}
/* fzgx:end fn_1_EF978 */

/* fzgx:begin fn_1_EFA18 */
// fn_1_EFA18: empty in retail (single blr).
void fn_1_EFA18(void) {
}
/* fzgx:end fn_1_EFA18 */

/* fzgx:begin fn_1_EFA4C */
// fn_1_EFA4C: empty in retail (single blr).
void fn_1_EFA4C(void) {
}
/* fzgx:end fn_1_EFA4C */

/* fzgx:begin ghost_reset */
extern u8 lbl_1_bss_7C8CE[70];
extern u32 lbl_1_data_3E52C;
extern void fn_1_F1D70(void);

void ghost_reset(void) {
    lbl_1_bss_7C8CE[0] = 0;
    lbl_1_data_3E52C = -1;
    fn_1_F1D70();
}
/* fzgx:end ghost_reset */

/* fzgx:begin fn_1_EFA88 */
// fn_1_EFA88: empty in retail (single blr).
void fn_1_EFA88(void) {
}
/* fzgx:end fn_1_EFA88 */

/* fzgx:begin fn_1_F0164 */
// fn_1_F0164: empty in retail (single blr).
void fn_1_F0164(void) {
}
/* fzgx:end fn_1_F0164 */

/* fzgx:begin ghost_pack_bits */
void ghost_pack_bits(u32 *out, const u32 *x, const u32 *y, const u32 *z) {
    out[0] = 0;
    out[1] = 0;
    out[0] += x[0] << 10;
    out[0] += y[0] >> 11;
    out[1] += y[0] << 21;
    out[1] += z[0];
}
/* fzgx:end ghost_pack_bits */

/* fzgx:begin fn_1_F1950 */
extern u32 lbl_1_bss_7ECFC[23];

void fn_1_F1950(void) {
    lbl_1_bss_7ECFC[0] = 1;
}
/* fzgx:end fn_1_F1950 */

/* fzgx:begin ghost_test_flag */
extern u32 lbl_1_bss_7E9E8[6];

u32 ghost_test_flag(s32 index) {
    if (index < 0x20) {
        return lbl_1_bss_7E9E8[0] & (1 << index);
    }
    if (index < 0x40) {
        return lbl_1_bss_7E9E8[1] & (1 << (index - 0x20));
    }
    if (index < 0x60) {
        return lbl_1_bss_7E9E8[2] & (1 << (index - 0x40));
    }
    return lbl_1_bss_7E9E8[3] & (1 << (index - 0x60));
}
/* fzgx:end ghost_test_flag */

/* fzgx:begin ghost_test_record_flag0 */
extern void fn_1_12EF80(s16 arg, s16 *out_a, s16 *out_b);
extern u8 lbl_1_bss_7F0C0[];

u32 ghost_test_record_flag0(s32 arg) {
    s16 a;
    s16 b;
    s32 index;

    fn_1_12EF80((s16)arg, &a, &b);
    index = (a - 1) * 6 + b;
    return lbl_1_bss_7F0C0[0x4938 + index] & 1;
}
/* fzgx:end ghost_test_record_flag0 */

/* fzgx:begin ghost_set_record_flag1 */
extern void fn_1_12EF80(s16 arg, s16 *out_a, s16 *out_b);
extern u8 lbl_1_bss_7F0C0[];

void ghost_set_record_flag1(s32 arg) {
    s16 a;
    s16 b;
    s32 index;

    fn_1_12EF80((s16)arg, &a, &b);
    index = (a - 1) * 6 + b;
    lbl_1_bss_7F0C0[0x4938 + index] |= 2;
}
/* fzgx:end ghost_set_record_flag1 */

/* fzgx:begin ghost_set_record_flag2 */
extern void fn_1_12EF80(s16 arg, s16 *out_a, s16 *out_b);
extern u8 lbl_1_bss_7F0C0[];

void ghost_set_record_flag2(s32 arg) {
    s16 a;
    s16 b;
    s32 index;

    fn_1_12EF80((s16)arg, &a, &b);
    index = (a - 1) * 6 + b;
    lbl_1_bss_7F0C0[0x4938 + index] |= 4;
}
/* fzgx:end ghost_set_record_flag2 */
