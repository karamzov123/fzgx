#include "types.h"

/* fzgx:begin fn_1_D6680 */
// fn_1_D6680: returns a constant.
int fn_1_D6680(void) {
    return 0;
}
/* fzgx:end fn_1_D6680 */

/* fzgx:begin fn_1_D6688 */
// fn_1_D6688: returns a constant.
int fn_1_D6688(void) {
    return 1;
}
/* fzgx:end fn_1_D6688 */

/* fzgx:begin fn_1_D6690 */
// fn_1_D6690: returns a constant.
int fn_1_D6690(void) {
    return 0;
}
/* fzgx:end fn_1_D6690 */

/* fzgx:begin fn_1_D6698 */
// fn_1_D6698: returns a constant.
int fn_1_D6698(void) {
    return 0;
}
/* fzgx:end fn_1_D6698 */

/* fzgx:begin fn_1_D66A0 */
// fn_1_D66A0: returns a constant.
int fn_1_D66A0(void) {
    return 0;
}
/* fzgx:end fn_1_D66A0 */

/* fzgx:begin fn_1_D66A8 */
// fn_1_D66A8: returns a constant.
int fn_1_D66A8(void) {
    return 0;
}
/* fzgx:end fn_1_D66A8 */

/* fzgx:begin fn_1_D66B0 */
// fn_1_D66B0: returns a constant.
int fn_1_D66B0(void) {
    return 0;
}
/* fzgx:end fn_1_D66B0 */

/* fzgx:begin fn_1_D66B8 */
// fn_1_D66B8: empty in retail (single blr).
void fn_1_D66B8(void) {
}
/* fzgx:end fn_1_D66B8 */

/* fzgx:begin fn_1_D66F4 */
// fn_1_D66F4: empty in retail (single blr).
void fn_1_D66F4(void) {
}
/* fzgx:end fn_1_D66F4 */

/* fzgx:begin fn_1_D6740 */
// fn_1_D6740: returns a constant.
int fn_1_D6740(void) {
    return 1;
}
/* fzgx:end fn_1_D6740 */

/* fzgx:begin fn_1_D76EC */
// fn_1_D76EC: empty in retail (single blr).
void fn_1_D76EC(void) {
}
/* fzgx:end fn_1_D76EC */

/* fzgx:begin fn_1_D76F0 */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern u32 lbl_1_data_2A7E0[62];

void fn_1_D76F0(const Vec3 *src, s16 index) {
    f32 *dst = (f32 *)lbl_1_data_2A7E0[15];
    dst[index * 3] = src->x;
    dst += index * 3;
    dst[1] = src->y;
    dst[2] = src->z;
}
/* fzgx:end fn_1_D76F0 */

/* fzgx:begin fn_1_D7724 */
typedef struct {
    u32 x;
    u32 y;
    u32 z;
} Vec3Bits;

extern u32 lbl_1_data_2A7E0[62];

void fn_1_D7724(Vec3Bits *dst, s16 index) {
    u32 *base = (u32 *)lbl_1_data_2A7E0[15];
    Vec3Bits *src = (Vec3Bits *)(base + index * 3);
    *dst = *src;
}
/* fzgx:end fn_1_D7724 */
