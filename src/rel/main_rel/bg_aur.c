#include "types.h"

/* fzgx:begin fn_1_151BDC */
extern u32 lbl_1_bss_8ED88;

void fn_1_151BDC(u32 value) {
    lbl_1_bss_8ED88 = value;
}
/* fzgx:end fn_1_151BDC */

/* fzgx:begin fn_1_153988 */
// fn_1_153988: empty in retail (single blr).
void fn_1_153988(void) {
}
/* fzgx:end fn_1_153988 */

/* fzgx:begin fn_1_1539D0 */
extern int fn_1_58C4(void);
extern void fn_1_5948(int);
extern void fn_1_627C(int);

// Calls initialization, then loops calling two functions for each item
void fn_1_1539D0(void) {
    int count = fn_1_58C4();
    int i;
    for (i = 0; i < count; i++) {
        fn_1_5948(i);
        fn_1_627C(i);
    }
}
/* fzgx:end fn_1_1539D0 */

/* fzgx:begin fn_1_153AAC */
// fn_1_153AAC: empty in retail (single blr).
void fn_1_153AAC(void) {
}
/* fzgx:end fn_1_153AAC */

/* fzgx:begin fn_1_153AB0 */
extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_153C60(u32 value);
extern void fn_1_1540B0(u32 value);
extern void fn_1_9AD88(void);

void fn_1_153AB0(void) {
    u32 value = lbl_1_data_2A7E0[15];

    fn_1_153C60(value);
    fn_1_1540B0(value);
    fn_1_9AD88();
}
/* fzgx:end fn_1_153AB0 */

/* fzgx:begin fn_1_153AF4 */
// fn_1_153AF4: empty in retail (single blr).
void fn_1_153AF4(void) {
}
/* fzgx:end fn_1_153AF4 */
