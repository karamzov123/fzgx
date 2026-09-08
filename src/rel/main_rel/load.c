#include "types.h"

/* fzgx:begin fn_1_45890 */
extern u32 lbl_1_bss_384D0[2];

void fn_1_45890(void) {
    lbl_1_bss_384D0[0] = 0;
}
/* fzgx:end fn_1_45890 */

/* fzgx:begin fn_1_45BE0 */
extern u32 lbl_1_bss_384C8;

void fn_1_45BE0(void) {
    lbl_1_bss_384C8 = 0;
}
/* fzgx:end fn_1_45BE0 */

/* fzgx:begin fn_1_46EA8 */
extern u32 lbl_1_bss_3DD28[179];

void fn_1_46EA8(u32 value) {
    lbl_1_bss_3DD28[0] = value;
}
/* fzgx:end fn_1_46EA8 */

/* fzgx:begin fn_1_46EC8 */
extern u32 lbl_1_bss_3DCDC[2];

u32 fn_1_46EC8(void) {
    return lbl_1_bss_3DCDC[0];
}
/* fzgx:end fn_1_46EC8 */

/* fzgx:begin fn_1_47A60 */
typedef struct {
    s32 value;
    s32 unk04;
} Fn147A60Entry;

extern Fn147A60Entry lbl_1_data_67F0[240];
extern void fn_1_48140(s32 value);
extern void fn_1_4DDC0(void);
extern void fn_1_4F724(void);

void fn_1_47A60(s16 index) {
    Fn147A60Entry *entry;
    s32 i;

    entry = &lbl_1_data_67F0[index * 10];
    for (i = 0; i < 10; i++, entry++) {
        if (entry->value != -1) {
            fn_1_48140(entry->value);
        } else {
            fn_1_4DDC0();
            fn_1_4F724();
            break;
        }
    }
}
/* fzgx:end fn_1_47A60 */
