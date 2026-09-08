#include "types.h"

/* fzgx:begin fn_1_D38A4 */
extern void fn_80071678(void);

void fn_1_D38A4(void) {
    fn_80071678();
}
/* fzgx:end fn_1_D38A4 */

/* fzgx:begin fn_1_D3BE8 */
extern u8 lbl_1_bss_7AD58[8];

// fn_1_D3BE8: increment the first byte in global array lbl_1_bss_7AD58
void fn_1_D3BE8(void) {
    lbl_1_bss_7AD58[0]++;
}
/* fzgx:end fn_1_D3BE8 */

/* fzgx:begin fn_1_D3C00 */
// fn_1_D3C00: empty in retail (single blr).
void fn_1_D3C00(void) {
}
/* fzgx:end fn_1_D3C00 */

/* fzgx:begin fn_1_D3E8C */
// fn_1_D3E8C: empty in retail (single blr).
void fn_1_D3E8C(void) {
}
/* fzgx:end fn_1_D3E8C */

/* fzgx:begin fn_1_D3E90 */
extern s16 camera_get_mode(void);
extern u32 fn_1_5910(void);
extern s32 fn_1_6EC0(u8 value);
extern u32 fn_1_864E8(s32 value);
extern s32 fn_1_7000(void);
extern void fn_1_681C(u8 value, void *out);
extern s32 fn_1_1FB80(void *value, s32 mode);

s32 fn_1_D3E90(void) {
    s16 value;
    u8 data[8];

    value = camera_get_mode();
    if (value != -1 && fn_1_6EC0((u8)fn_1_5910()) != 0 &&
        (fn_1_864E8((s16)value) & 0x20) != 0) {
        return 1;
    }

    if (fn_1_7000() != 0) {
        return 1;
    }

    fn_1_681C((u8)fn_1_5910(), data);
    return fn_1_1FB80(data, 1);
}
/* fzgx:end fn_1_D3E90 */

/* fzgx:begin fn_1_D3F24 */
extern void fn_1_103090(void *ptr);

void fn_1_D3F24(void *base) {
    s32 value;
    s32 count;
    char *ptr;

    value = 1;
    ptr = (char *)base + 0x10000;
    count = *(s32 *)(ptr - 0x1820);
    ptr -= 0x181c;

    while (count > 0) {
        *(s32 *)(ptr + 0x68) = value;
        fn_1_103090(ptr);
        count--;
        ptr += 0xac;
    }
}
/* fzgx:end fn_1_D3F24 */

/* fzgx:begin fn_1_D3F88 */
extern void fn_1_1030A4(void *ptr);

// Iterate through items in a structure, calling fn_1_1030A4 on each.
void fn_1_D3F88(void *base) {
    s32 count;
    char *ptr;
    
    ptr = (char *)base + 0x10000;
    count = *(s32 *)(ptr - 0x1820);
    ptr -= 0x181c;
    
    while (count > 0) {
        fn_1_1030A4(ptr);
        count--;
        ptr += 0xac;
    }
}
/* fzgx:end fn_1_D3F88 */

/* fzgx:begin fn_1_D5958 */
extern void fn_1_D550C(void *);

// fn_1_D5958: iterate through 4 structures, calling fn_1_D550C on each.
void fn_1_D5958(void *arg) {
    void *ptr = arg;
    int i = 0;
    for (; i < 4; i++) {
        fn_1_D550C((char *)ptr + 0xc07c);
        ptr = (char *)ptr + 0x30;
    }
}
/* fzgx:end fn_1_D5958 */

/* fzgx:begin fn_1_D5C68 */
// fn_1_D5C68: returns a constant.
int fn_1_D5C68(void) {
    return 0;
}
/* fzgx:end fn_1_D5C68 */
