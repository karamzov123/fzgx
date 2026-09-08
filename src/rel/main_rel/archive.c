#include "types.h"

/* fzgx:begin fn_1_12A32C */
extern void fn_1_12A350(void *arg0, s32 arg1);

void fn_1_12A32C(void *arg0) {
    fn_1_12A350(arg0, 0);
}
/* fzgx:end fn_1_12A32C */

/* fzgx:begin fn_1_12C000 */
extern void fn_1_8D168(void *arg);

void fn_1_12C000(void *arg, s16 count) {
    u8 *p;
    s16 i;

    p = (u8 *)arg;
    i = 0;
    while (i < count) {
        fn_1_8D168(p);
        p += 100;
        i++;
    }
}
/* fzgx:end fn_1_12C000 */

/* fzgx:begin fn_1_12C0EC */
typedef struct {
    u16 unk0;
    u32 unk4;
    u8 unk8;
    u8 pad9[3];
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
} FnData;

void fn_1_12C0EC(FnData *arg) {
    arg->unk18 = 0;
    arg->unk14 = 0;
    arg->unk10 = 0;
    arg->unkC = 0;
    arg->unk8 = 0;
    arg->unk4 = 0;
    arg->unk0 = 0;
}
/* fzgx:end fn_1_12C0EC */

/* fzgx:begin fn_1_12C6BC */
extern void fn_1_14DBCC(void *arg);

void fn_1_12C6BC(void *arg) {
    u8 *p;
    s16 i;

    p = (u8 *)arg;
    i = 0;
    while (i < 9) {
        fn_1_14DBCC(p);
        p += 0xa20;
        i++;
    }
}
/* fzgx:end fn_1_12C6BC */

/* fzgx:begin fn_1_12D354 */
extern char lbl_1_data_40798[];
extern u32 lbl_1_bss_8B56C[24];
extern void fn_8000C49C(char *format, ...);

void fn_1_12D354(void *arg0, void *arg1, void *arg2) {
    char *base = lbl_1_data_40798;
    u32 i;

    if (arg2 == 0) {
        fn_8000C49C(base + 0x358, 0x388, base + 0x368);
    }

    for (i = 0; i < 12; i++) {
        if (lbl_1_bss_8B56C[i * 2] == 0) {
            lbl_1_bss_8B56C[i * 2] = (u32)arg2;
            lbl_1_bss_8B56C[i * 2 + 1] = 0;
            return;
        }
    }

    fn_8000C49C(base + 0x358, 0x394, base + 0x390);
}
/* fzgx:end fn_1_12D354 */

/* fzgx:begin fn_1_12E424 */
extern u32 lbl_1_bss_8B3A0[83];
extern u32 fn_1_95120(void *arg);

u8 fn_1_12E424(void *arg0, u8 *arg1) {
    if (lbl_1_bss_8B3A0[37] & (1u << 31)) {
        return 1;
    }
    return (u8)fn_1_95120(arg1 + 0x148);
}
/* fzgx:end fn_1_12E424 */

/* fzgx:begin fn_1_12F10C */
extern u32 lbl_1_bss_8B5EC;

void fn_1_12F10C(u32 value) {
    lbl_1_bss_8B5EC = value;
}
/* fzgx:end fn_1_12F10C */

/* fzgx:begin fn_1_12F128 */
extern u32 lbl_801A66A0;
extern u32 lbl_1_bss_8B3A0[83];

void fn_1_12F128(s16 index) {
    lbl_1_bss_8B3A0[index + 56] = lbl_801A66A0;
}
/* fzgx:end fn_1_12F128 */

/* fzgx:begin fn_1_12F194 */
extern u32 lbl_1_bss_8B3A0[83];

void fn_1_12F194(void) {
    lbl_1_bss_8B3A0[72] = 0;
    lbl_1_bss_8B3A0[60] = 0;
    lbl_1_bss_8B3A0[64] = 0;
    lbl_1_bss_8B3A0[68] = 0;
    lbl_1_bss_8B3A0[56] = 0;
    lbl_1_bss_8B3A0[61] = 0;
    lbl_1_bss_8B3A0[65] = 0;
    lbl_1_bss_8B3A0[69] = 0;
    lbl_1_bss_8B3A0[57] = 0;
    lbl_1_bss_8B3A0[62] = 0;
    lbl_1_bss_8B3A0[66] = 0;
    lbl_1_bss_8B3A0[70] = 0;
    lbl_1_bss_8B3A0[58] = 0;
    lbl_1_bss_8B3A0[63] = 0;
    lbl_1_bss_8B3A0[67] = 0;
    lbl_1_bss_8B3A0[71] = 0;
    lbl_1_bss_8B3A0[59] = 0;
}
/* fzgx:end fn_1_12F194 */

/* fzgx:begin fn_1_12F308 */
// fn_1_12F308: empty in retail (single blr).
void fn_1_12F308(void) {
}
/* fzgx:end fn_1_12F308 */

/* fzgx:begin fn_1_130F98 */
extern s32 lbl_1_bss_8CA44[10];
extern s32 lbl_1_bss_8CA40;
extern void (*lbl_1_data_40D50[8])(void);

void fn_1_130F98(void) {
    if (lbl_1_bss_8CA44[0] >= 0) {
        s32 value = lbl_1_bss_8CA44[0];

        lbl_1_bss_8CA44[0] = -1;
        lbl_1_bss_8CA40 = value;
    }

    if (lbl_1_bss_8CA40 >= 0) {
        lbl_1_data_40D50[lbl_1_bss_8CA40]();
    }
}
/* fzgx:end fn_1_130F98 */

/* fzgx:begin fn_1_131000 */
// fn_1_131000: empty in retail (single blr).
void fn_1_131000(void) {
}
/* fzgx:end fn_1_131000 */

/* fzgx:begin fn_1_1310FC */
extern u32 lbl_1_bss_8CA40;
extern void fn_1_C2454(void);

void fn_1_1310FC(void) {
    u8* ptr = (u8*)&lbl_1_bss_8CA40;
    
    if (*(s32*)(ptr + 0x8) == -1) {
        *(u32*)(ptr + 0x0) = (u32)-1;
    } else {
        fn_1_C2454();
        *(u32*)(ptr + 0x4) = 1;
        *(u8*)(ptr + 0x30) = 0;
    }
}
/* fzgx:end fn_1_1310FC */

/* fzgx:begin fn_1_131154 */
extern s32 fn_1_C24A4(void);
extern u8 fn_1_B7C00(void);
extern s32 lbl_1_bss_8CA40;

void fn_1_131154(void) {
    if (fn_1_C24A4() == 0 && fn_1_B7C00() == 0) {
        lbl_1_bss_8CA40 = -1;
    }
}
/* fzgx:end fn_1_131154 */
