#include "types.h"

/* fzgx:begin fn_1_7BE94 */
extern u16 lbl_1_bss_6D7C0[20];

u16 fn_1_7BE94(void) {
    return lbl_1_bss_6D7C0[0] - 1;
}
/* fzgx:end fn_1_7BE94 */

/* fzgx:begin fn_1_7BF9C */
typedef struct {
    void *unk_0;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
    s8 unk_7;
    u8 unk_8;
    u8 unk_9[3];
    void *unk_c;
    void *unk_10;
    void *unk_14;
    void *unk_18;
    void *unk_1c;
    u32 unk_20;
    u8 unk_24;
} LightState;

extern char lbl_1_data_1EEB0[12];
extern void *lbl_801A6410;
extern LightState *lbl_1_bss_6D7E8;
extern void *fn_1_4630(void *, int, void *, int);
extern void fn_800793D4(void *, int, int);
extern void fn_1_7C13C(void *);
extern void fn_1_7C1E8(void *);
extern u8 fn_1_816E8(void *, u8);
extern const f32 lbl_1_rodata_33A8;
extern void fn_1_495D8(int, int, f32);
extern void fn_1_495FC(void);

void fn_1_7BF9C(void *arg, s8 flag) {
    u8 *base;
    void *resource_18;
    void *resource_1c;
    void *resource_c;

    base = (u8 *)&lbl_1_data_1EEB0;
    lbl_1_bss_6D7E8 = fn_1_4630(
        lbl_801A6410, 0x2c, base + 0xac, 0x33);
    resource_18 = fn_1_4630(
        lbl_801A6410, 4, base + 0xac, 0x34);
    resource_1c = fn_1_4630(
        *(void **)&lbl_801A6410, 0x18, base + 0xac, 0x35);
    resource_c = fn_1_4630(
        *(void **)&lbl_801A6410, 0xa4, base + 0xac, 0x36);

    fn_800793D4(resource_c, 0xff, 0xa4);
    fn_1_7C13C(arg);
    fn_1_7C1E8(resource_1c);

    lbl_1_bss_6D7E8->unk_0 = arg;
    lbl_1_bss_6D7E8->unk_4 = 2;
    lbl_1_bss_6D7E8->unk_6 = 2;
    lbl_1_bss_6D7E8->unk_24 = 0;
    lbl_1_bss_6D7E8->unk_8 = fn_1_816E8(
        (u8 *)lbl_1_bss_6D7E8->unk_0 +
            lbl_1_bss_6D7E8->unk_7 * 0x440,
        lbl_1_bss_6D7E8->unk_24);

    lbl_1_bss_6D7E8->unk_c = resource_c;
    lbl_1_bss_6D7E8->unk_10 = base + 0x48;
    lbl_1_bss_6D7E8->unk_14 = base + 0x94;
    lbl_1_bss_6D7E8->unk_18 = resource_18;
    lbl_1_bss_6D7E8->unk_1c = resource_1c;

    if (flag == 0) {
        lbl_1_bss_6D7E8->unk_20 |= 0x40000000;
    }

    fn_1_495D8(2, 2, lbl_1_rodata_33A8);
    fn_1_495FC();
}
/* fzgx:end fn_1_7BF9C */
