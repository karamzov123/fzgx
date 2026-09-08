#include "types.h"

/* fzgx:begin fn_1_102FD0 */
extern void fn_1_105744(void);
extern u32 lbl_1_bss_3BE0;
extern u8 lbl_1_data_3F244[64];
extern void fn_1_106DB4(void);
extern void fn_1_9D0EC(u32, u32, void*, void*);
extern void fn_1_103AA8(void);
extern u32 lbl_1_bss_85288[2];

// fn_1_102FD0: Initialize some game state
void fn_1_102FD0(void) {
    u32 base;
    u32 field1;
    u32 field2;
    
    fn_1_105744();
    
    base = lbl_1_bss_3BE0;
    field1 = *(u32*)(base + 0x54);
    field2 = *(u32*)(base + 0x48);
    
    fn_1_9D0EC(field1, field2, &lbl_1_data_3F244[0], &fn_1_106DB4);
    fn_1_103AA8();
    
    lbl_1_bss_85288[0] = -1;
    lbl_1_bss_85288[1] = -1;
}
/* fzgx:end fn_1_102FD0 */

/* fzgx:begin fn_1_10302C */
// fn_1_10302C: empty in retail (single blr).
void fn_1_10302C(void) {
}
/* fzgx:end fn_1_10302C */

/* fzgx:begin fn_1_103054 */
// fn_1_103054: empty in retail (single blr).
void fn_1_103054(void) {
}
/* fzgx:end fn_1_103054 */

/* fzgx:begin fn_1_103058 */
extern s16 lbl_1_bss_960;
extern void fn_1_10576C(void);

void fn_1_103058(void) {
    if (lbl_1_bss_960 != 10 && lbl_1_bss_960 != 14) {
        fn_1_10576C();
    }
}
/* fzgx:end fn_1_103058 */
