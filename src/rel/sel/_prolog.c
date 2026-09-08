#include "types.h"

/* fzgx:begin fn_10_0 */
struct Fn10Data {
    u8 pad[0x18];
    s16 value_18;
    s16 value_1a;
};

extern struct Fn10Data lbl_10_bss_0;
extern int fn_1_152970(int, int, int, int, struct Fn10Data *, int, int);

s16 fn_10_0(int first, int second, int third, int fourth) {
    return (s16)fn_1_152970(second, first, third, fourth,
        (struct Fn10Data *)((u8 *)&lbl_10_bss_0 + 0x1c),
        lbl_10_bss_0.value_1a, lbl_10_bss_0.value_18);
}
/* fzgx:end fn_10_0 */

/* fzgx:begin fn_10_AB8 */
extern u32 lbl_10_bss_49384;
extern u32 lbl_10_bss_49388;

extern void fn_1_435C(u32);
extern void fn_1_407C(void);
extern void fn_1_3CC4(void);

void fn_10_AB8(void) {
    fn_1_435C(lbl_10_bss_49384);
    fn_1_407C();
    fn_1_3CC4();
    fn_1_435C(lbl_10_bss_49388);
    fn_1_407C();
}
/* fzgx:end fn_10_AB8 */
