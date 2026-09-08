#include "types.h"
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u16 lbl_1_bss_96A;
s32 fn_1_4C10(void);
extern u32 lbl_9_bss_C;
extern u32 lbl_9_bss_10;
void fn_1_435C(u32);
void fn_1_407C(void);
void fn_1_3CC4(void);

/* fzgx:begin fn_9_298 */
struct Struct_lbl_1_bss_D58 {
    u8 pad[8];
    u16 unk8;
};

void fn_9_298(void) {
    if (fn_1_4C10() == 0) {
        if ((lbl_1_bss_D58.unk8 >> 12) & 1) {
            lbl_1_bss_96A = 0x75;
        }
    }
}
/* fzgx:end fn_9_298 */

/* fzgx:begin fn_9_2E0 */
void fn_9_2E0(void) {
    fn_1_435C(lbl_9_bss_C);
    fn_1_407C();
    fn_1_3CC4();
    fn_1_435C(lbl_9_bss_10);
    fn_1_407C();
}
/* fzgx:end fn_9_2E0 */
