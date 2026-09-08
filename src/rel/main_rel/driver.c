#include "types.h"

/* fzgx:begin fn_1_A8DC4 */
extern u32 lbl_1_bss_6F640[2];

u32 fn_1_A8DC4(void) {
    return lbl_1_bss_6F640[0];
}
/* fzgx:end fn_1_A8DC4 */

/* fzgx:begin fn_1_A8E78 */
extern void fn_1_A9420(int);
extern void fn_1_A948C(int);

// fn_1_A8E78: Take an argument, call fn_1_A9420(0), then fn_1_A948C with original arg.

void fn_1_A8E78(int arg) {
    fn_1_A9420(0);
    fn_1_A948C(arg);
}
/* fzgx:end fn_1_A8E78 */

/* fzgx:begin fn_1_A96EC */
// fn_1_A96EC: main_rel .text:0x000A96EC size 0x10
// Zero the first element of lbl_1_data_3599C

extern u32 lbl_1_data_3599C[15];

void fn_1_A96EC(void) {
    lbl_1_data_3599C[0] = 0;
}
/* fzgx:end fn_1_A96EC */

/* fzgx:begin fn_1_A96FC */
extern u32 fn_1_451C(void);
extern void fn_8008069C(void *buffer, char *format, ...);
extern void fn_1_A5AA0(void *buffer, void *destination);

extern s16 lbl_1_bss_960;
extern u32 lbl_1_bss_71670;
extern u32 lbl_1_data_35A70[18];
extern char lbl_1_data_35AB8[11];
extern u8 lbl_1_bss_71674[8];

void fn_1_A96FC(void) {
    u8 buffer[0x48];

    lbl_1_bss_71670 = fn_1_451C();
    fn_8008069C(buffer, lbl_1_data_35AB8,
                lbl_1_data_35A70[lbl_1_bss_960]);
    fn_1_A5AA0(buffer, lbl_1_bss_71674);
}
/* fzgx:end fn_1_A96FC */

/* fzgx:begin fn_1_A9764 */
extern void (*lbl_1_bss_7167C)(void);

void fn_1_A9764(void) {
    lbl_1_bss_7167C();
}
/* fzgx:end fn_1_A9764 */

/* fzgx:begin fn_1_A9790 */
extern void (*lbl_1_bss_71680)(void);

void fn_1_A9790(void) {
    lbl_1_bss_71680();
}
/* fzgx:end fn_1_A9790 */

/* fzgx:begin fn_1_A97E4 */
extern void (*lbl_1_bss_71684)(void);

void fn_1_A97E4(void) {
    lbl_1_bss_71684();
}
/* fzgx:end fn_1_A97E4 */

/* fzgx:begin fn_1_A9810 */
extern void (*lbl_1_bss_71688)(void);

void fn_1_A9810(void) {
    lbl_1_bss_71688();
}
/* fzgx:end fn_1_A9810 */

/* fzgx:begin fn_1_AAF18 */
// fn_1_AAF18: empty in retail (single blr).
void fn_1_AAF18(void) {
}
/* fzgx:end fn_1_AAF18 */

/* fzgx:begin fn_1_AB434 */
// fn_1_AB434: empty in retail (single blr).
void fn_1_AB434(void) {
}
/* fzgx:end fn_1_AB434 */

/* fzgx:begin fn_1_AB458 */
// fn_1_AB458: empty in retail (single blr).
void fn_1_AB458(void) {
}
/* fzgx:end fn_1_AB458 */
