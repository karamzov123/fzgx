#include "types.h"

/* fzgx:begin fn_1_7D694 */
extern u32 lbl_1_bss_6D7E8[2];

typedef struct {
    u8 _pad4[4];
    s16 value;
} Entry;

typedef struct {
    u8 _pad32c[0x32c];
    Entry *entry;
} Object;

typedef struct {
    u8 _pad7[7];
    s8 value;
} GlobalEntry;

void fn_1_7D694(Object *obj) {
    Entry *entry = obj->entry;
    GlobalEntry *global = (GlobalEntry *)lbl_1_bss_6D7E8[0];
    if (entry->value == global->value) {
        return;
    }
}
/* fzgx:end fn_1_7D694 */

/* fzgx:begin fn_1_7EAE8 */
// fn_1_7EAE8: main_rel .text:0x0007EAE8 size 0x24
// Wrapper that calls fn_1_5634C with argument 0.

extern void fn_1_5634C(u32 arg0);

void fn_1_7EAE8(void) {
    fn_1_5634C(0);
}
/* fzgx:end fn_1_7EAE8 */

/* fzgx:begin fn_1_7F1E8 */
extern void fn_1_7ECB8(u32 arg0, u32 arg1, u32 arg2);

void fn_1_7F1E8(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0x80000000); // fzgx-allow: A1 retail sentinel
}
/* fzgx:end fn_1_7F1E8 */

/* fzgx:begin fn_1_7F230 */
extern void fn_1_7ECB8(u32 arg0, u32 arg1, u32 arg2);

void fn_1_7F230(u32 arg0, u32 arg1) {
    fn_1_7ECB8(arg0, arg1, 0);
}
/* fzgx:end fn_1_7F230 */
