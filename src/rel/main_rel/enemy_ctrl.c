#include "types.h"

/* fzgx:begin fn_1_CC27C */
// fn_1_CC27C: empty in retail (single blr).
void fn_1_CC27C(void) {
}
/* fzgx:end fn_1_CC27C */

/* fzgx:begin fn_1_D07AC */
extern u8 lbl_1_bss_7ACA0[48];

void fn_1_D07AC(u32 index, u32 value) {
    ((u32 *)lbl_1_bss_7ACA0)[(index & 0xff) * 3] = value;
}
/* fzgx:end fn_1_D07AC */

/* fzgx:begin fn_1_D0DA8 */
extern u16 lbl_1_data_3D648[20];

u8* fn_1_D0DA8(void) {
    return (u8*)lbl_1_data_3D648 + 0x6;
}
/* fzgx:end fn_1_D0DA8 */

/* fzgx:begin fn_1_D0E08 */
extern u16 lbl_1_data_3D648[20];

u8* fn_1_D0E08(void) {
    return (u8*)lbl_1_data_3D648 + 0x16;
}
/* fzgx:end fn_1_D0E08 */

/* fzgx:begin fn_1_D0E18 */
extern u16 lbl_1_data_3D648[20];

u8* fn_1_D0E18(void) {
    return (u8*)lbl_1_data_3D648 + 0x18;
}
/* fzgx:end fn_1_D0E18 */

/* fzgx:begin fn_1_D0E28 */
extern u16 lbl_1_data_3D648[20];

u16* fn_1_D0E28(void) {
    return lbl_1_data_3D648;
}
/* fzgx:end fn_1_D0E28 */

/* fzgx:begin fn_1_D0E34 */
extern u16 lbl_1_data_3D648[20];

u8* fn_1_D0E34(void) {
    return (u8*)lbl_1_data_3D648 + 0x20;
}
/* fzgx:end fn_1_D0E34 */

/* fzgx:begin fn_1_D0E54 */
extern u16 lbl_1_data_3D648[20];

u8* fn_1_D0E54(void) {
    return (u8*)lbl_1_data_3D648 + 0x22;
}
/* fzgx:end fn_1_D0E54 */
