#include "types.h"
extern struct Obj *fn_3_14074(void);
extern u8 lbl_3_bss_A2408[8];
extern void *fn_1_45D0(void *, u32, char *, u32);
extern void fn_80008BEC(void *, u32, u32);
extern void fn_3_17100(void);

/* fzgx:begin fn_3_1552C */
struct Obj {
    u8 pad[0x10];
    u16 width;
    u16 height;
};

extern struct Obj *fn_3_14074(void);
extern u8 lbl_3_bss_A23EC[28];
extern char lbl_3_data_35B0[9];
extern u8 lbl_3_bss_A2408[8];
extern void *lbl_801A6410;
extern void *fn_1_45D0(void *, u32, char *, u32);
extern void fn_80008BEC(void *, u32, u32);

void fn_3_1552C(void) {
    struct Obj *ptr;

    ptr = fn_3_14074();
    ptr->width = 0x20;
    ptr->height = 0x20;
    *(u32 *)(lbl_3_bss_A23EC + 0xC) = 0x20000000;
    *(u8 *)(lbl_3_bss_A23EC + 0x11) = 1;
    *(u8 *)(lbl_3_bss_A23EC + 0x16) = 0;
    *(u8 *)(lbl_3_bss_A23EC + 0x0) = 0;
    *(u32 *)(lbl_3_bss_A23EC + 0x4) =
        (u32)fn_1_45D0(lbl_801A6410, 0x2000, lbl_3_data_35B0, 0x4A);
    *(u32 *)(lbl_3_bss_A23EC + 0x8) =
        (u32)fn_1_45D0(lbl_801A6410, 0x2000, lbl_3_data_35B0, 0x4B);
    fn_80008BEC(*(void **)(lbl_3_bss_A23EC + 0x4), 0, 0x2000);
    fn_80008BEC(*(void **)(lbl_3_bss_A23EC + 0x8), 0, 0x2000);
    *lbl_3_bss_A2408 = 0;
}
/* fzgx:end fn_3_1552C */

/* fzgx:begin fn_3_1560C */
extern u8 lbl_3_bss_A23EC[28];

void fn_3_1560C(void) {
    *(u32 *)(lbl_3_bss_A23EC + 0xC) = (u32)1 << 31;
}
/* fzgx:end fn_3_1560C */

/* fzgx:begin fn_3_15620 */
extern u8 lbl_3_bss_A23EC[28];

void fn_3_15620(void) {
    *(u8 *)(lbl_3_bss_A23EC + 0x10) = 0;
    *(u32 *)(lbl_3_bss_A23EC + 0xc) = 0x40000000;
}
/* fzgx:end fn_3_15620 */

/* fzgx:begin fn_3_1563C */
extern u8 lbl_3_bss_A23EC[28];
extern char lbl_3_data_35B0[9];
extern u32 lbl_801A6410;

extern void fn_1_46B4(u32, u32, char *, int);

void fn_3_1563C(void) {
    fn_1_46B4(lbl_801A6410, *(u32 *)(lbl_3_bss_A23EC + 0x4),
              lbl_3_data_35B0, 0x71);
    fn_1_46B4(lbl_801A6410, *(u32 *)(lbl_3_bss_A23EC + 0x8),
              lbl_3_data_35B0, 0x72);
}
/* fzgx:end fn_3_1563C */

/* fzgx:begin fn_3_156A8 */
extern u8 lbl_3_bss_A23EC[28];

void fn_3_156A8(void) {
    u8 value = (lbl_3_bss_A23EC[0x11] & 0x7f) << 1;
    lbl_3_bss_A23EC[0x11] = value;
    if (value > 6) {
        lbl_3_bss_A23EC[0x11] = 1;
    }
}
/* fzgx:end fn_3_156A8 */

/* fzgx:begin fn_3_170E0 */
void fn_3_170E0(void) {
    fn_3_17100();
}
/* fzgx:end fn_3_170E0 */

/* fzgx:begin fn_3_17820 */
extern u16 lbl_3_bss_A2410[20];

u32 fn_3_17820(void) {
    return *(u32 *)&lbl_3_bss_A2410[2];
}
/* fzgx:end fn_3_17820 */

/* fzgx:begin fn_3_17830 */
typedef struct CustomizeState {
    u8 _pad0[8];
    u32 flags;
    u8 _pad1[16];
    f32 field_1c;
} CustomizeState;

extern CustomizeState lbl_3_bss_A2410;
extern const f32 lbl_3_rodata_5FC[3];

void fn_3_17830(void) {
    lbl_3_bss_A2410.field_1c = lbl_3_rodata_5FC[0];
    lbl_3_bss_A2410.flags |= 0x20000000u;
}
/* fzgx:end fn_3_17830 */
