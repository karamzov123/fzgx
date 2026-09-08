#include "types.h"

/* fzgx:begin fn_8_AA8 */
extern struct fn_8_AA8_lbl_8_bss_234 lbl_8_bss_234;
extern struct fn_8_AA8_lbl_8_bss_238 lbl_8_bss_238;
extern u32 fn_1_3CC4(u32);
extern u32 fn_1_407C(u32);
extern u32 fn_1_435C(u32);

struct fn_8_AA8_lbl_8_bss_234 {
    u32 unk_0;
};
struct fn_8_AA8_lbl_8_bss_238 {
    u32 unk_0;
};

void fn_8_AA8(void) {
    u32 t0, t1, t3;
    t0 = fn_1_435C(lbl_8_bss_234.unk_0);
    t1 = fn_1_407C(t0);
    fn_1_3CC4(t1);
    t3 = fn_1_435C(lbl_8_bss_238.unk_0);
    fn_1_407C(t3);
}
/* fzgx:end fn_8_AA8 */

/* fzgx:begin fn_8_BD8 */
// fn_8_BD8: empty in retail (single blr).
void fn_8_BD8(void) {
}
/* fzgx:end fn_8_BD8 */

/* fzgx:begin fn_8_BDC */
// fn_8_BDC: empty in retail (single blr).
void fn_8_BDC(void) {
}
/* fzgx:end fn_8_BDC */

/* fzgx:begin fn_8_BE0 */
// fn_8_BE0: empty in retail (single blr).
void fn_8_BE0(void) {
}
/* fzgx:end fn_8_BE0 */

/* fzgx:begin fn_8_1304 */
extern u32 fn_1_48140(u32);
extern u32 fn_1_A1364(u32);
extern u32 fn_1_A8F78(void);
extern u8 lbl_8_bss_241;
extern u8 lbl_8_bss_2;

void fn_8_1304(void) {
    u32 t1;
    lbl_8_bss_2 = 0;
    lbl_8_bss_241 = 0;
    fn_1_A8F78();
    t1 = fn_1_48140(142);
    fn_1_A1364(t1);
}
/* fzgx:end fn_8_1304 */

/* fzgx:begin fn_8_1C08 */
// fn_8_1C08: empty in retail (single blr).
void fn_8_1C08(void) {
}
/* fzgx:end fn_8_1C08 */

/* fzgx:begin fn_8_1C0C */
// fn_8_1C0C: empty in retail (single blr).
void fn_8_1C0C(void) {
}
/* fzgx:end fn_8_1C0C */

/* fzgx:begin fn_8_1C10 */
// fn_8_1C10: empty in retail (single blr).
void fn_8_1C10(void) {
}
/* fzgx:end fn_8_1C10 */

/* fzgx:begin fn_8_1C14 */
// fn_8_1C14: empty in retail (single blr).
void fn_8_1C14(void) {
}
/* fzgx:end fn_8_1C14 */

/* fzgx:begin fn_8_1F64 */
extern u8 lbl_8_bss_2;

void fn_8_1F64(void) {
    lbl_8_bss_2 = 0;
}
/* fzgx:end fn_8_1F64 */

/* fzgx:begin fn_8_2120 */
// fn_8_2120: empty in retail (single blr).
void fn_8_2120(void) {
}
/* fzgx:end fn_8_2120 */

/* fzgx:begin fn_8_4E8C */
// fn_8_4E8C: empty in retail (single blr).
void fn_8_4E8C(void) {
}
/* fzgx:end fn_8_4E8C */

/* fzgx:begin fn_8_516C */
extern struct fn_8_516C_lbl_1_bss_7C850 lbl_1_bss_7C850;
extern struct fn_8_516C_lbl_801A6410 lbl_801A6410;
extern struct fn_8_516C_lbl_8_bss_0 lbl_8_bss_0;
extern u32 fn_1_426C(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, u32, u32);
extern u32 fn_8_69D8(u32, u32);
extern u32 lbl_8_data_434;

struct fn_8_516C_lbl_8_bss_0 {
    u8 pad_0[0x8];
    u32 unk_8;
    u8 pad_C[0x22C];
    u32 unk_238;
    u8 pad_23C[0x1];
    u8 unk_23D;
    u8 pad_23E[0x4A];
    u32 unk_288;
};
struct fn_8_516C_lbl_801A6410 {
    u32 unk_0;
};
struct fn_8_516C_lbl_1_bss_7C850 {
    u32 unk_0;
};

void fn_8_516C(void) {
    struct fn_8_516C_lbl_8_bss_0 *p_lbl_8_bss_0;
    u32 v0;
    p_lbl_8_bss_0 = (struct fn_8_516C_lbl_8_bss_0 *)&lbl_8_bss_0;
    fn_1_435C(p_lbl_8_bss_0->unk_238);
    if ((s32)p_lbl_8_bss_0->unk_288 != 0) {
    fn_1_426C(p_lbl_8_bss_0->unk_288);
    p_lbl_8_bss_0->unk_288 = 0;
    }
    if (p_lbl_8_bss_0->unk_8 != 0) {
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_8_bss_0->unk_8, (u32)&lbl_8_data_434, 3405);
    p_lbl_8_bss_0->unk_8 = 0;
    }
    v0 = lbl_1_bss_7C850.unk_0;
    lbl_1_bss_7C850.unk_0 = (v0 + 1);
    if (p_lbl_8_bss_0->unk_23D == 0) {
    fn_8_69D8(v0, (u32)&lbl_1_bss_7C850);
    }
}
/* fzgx:end fn_8_516C */

/* fzgx:begin fn_8_520C */
// fn_8_520C: empty in retail (single blr).
void fn_8_520C(void) {
}
/* fzgx:end fn_8_520C */

/* fzgx:begin fn_8_5210 */
// fn_8_5210: empty in retail (single blr).
void fn_8_5210(void) {
}
/* fzgx:end fn_8_5210 */

/* fzgx:begin fn_8_5214 */
// fn_8_5214: empty in retail (single blr).
void fn_8_5214(void) {
}
/* fzgx:end fn_8_5214 */

/* fzgx:begin fn_8_5514 */
extern struct fn_8_5514_lbl_1_bss_7C850 lbl_1_bss_7C850;
extern struct fn_8_5514_lbl_801A6410 lbl_801A6410;
extern struct fn_8_5514_lbl_8_bss_0 lbl_8_bss_0;
extern u32 fn_1_426C(u32);
extern u32 fn_1_435C(u32);
extern u32 fn_1_46B4(u32, u32, u32, u32);
extern u32 fn_8_6C50(u32, u32);
extern u32 lbl_8_data_434;

struct fn_8_5514_lbl_8_bss_0 {
    u8 pad_0[0x8];
    u32 unk_8;
    u8 pad_C[0x22C];
    u32 unk_238;
    u8 pad_23C[0x1];
    u8 unk_23D;
    u8 pad_23E[0x4A];
    u32 unk_288;
    u32 unk_28C;
};
struct fn_8_5514_lbl_801A6410 {
    u32 unk_0;
};
struct fn_8_5514_lbl_1_bss_7C850 {
    u32 unk_0;
};

void fn_8_5514(void) {
    struct fn_8_5514_lbl_8_bss_0 *p_lbl_8_bss_0;
    u32 v0;
    p_lbl_8_bss_0 = (struct fn_8_5514_lbl_8_bss_0 *)&lbl_8_bss_0;
    fn_1_435C(p_lbl_8_bss_0->unk_238);
    if ((s32)p_lbl_8_bss_0->unk_28C != 0) {
    fn_1_426C(p_lbl_8_bss_0->unk_28C);
    p_lbl_8_bss_0->unk_28C = 0;
    }
    if ((s32)p_lbl_8_bss_0->unk_288 != 0) {
    fn_1_426C(p_lbl_8_bss_0->unk_288);
    p_lbl_8_bss_0->unk_288 = 0;
    }
    if (p_lbl_8_bss_0->unk_8 != 0) {
    fn_1_46B4(lbl_801A6410.unk_0, p_lbl_8_bss_0->unk_8, (u32)&lbl_8_data_434, 3560);
    p_lbl_8_bss_0->unk_8 = 0;
    }
    v0 = lbl_1_bss_7C850.unk_0;
    lbl_1_bss_7C850.unk_0 = (v0 + 1);
    if (p_lbl_8_bss_0->unk_23D == 0) {
    fn_8_6C50(v0, (u32)&lbl_1_bss_7C850);
    }
}
/* fzgx:end fn_8_5514 */

/* fzgx:begin fn_8_55CC */
extern u32 fn_8_D024(void);
extern u8 lbl_8_bss_23D;
extern u8 lbl_8_bss_242;

void fn_8_55CC(void) {
    if (lbl_8_bss_23D == 0) {
    lbl_8_bss_242 = 0;
    fn_8_D024();
    }
}
/* fzgx:end fn_8_55CC */

/* fzgx:begin fn_8_5810 */
extern u32 fn_8_D5F4(void);
extern u8 lbl_8_bss_23D;

void fn_8_5810(void) {
    if (lbl_8_bss_23D == 0) {
    fn_8_D5F4();
    }
}
/* fzgx:end fn_8_5810 */

/* fzgx:begin fn_8_5C4C */
// fn_8_5C4C: empty in retail (single blr).
void fn_8_5C4C(void) {
}
/* fzgx:end fn_8_5C4C */

/* fzgx:begin fn_8_5C50 */
// fn_8_5C50: empty in retail (single blr).
void fn_8_5C50(void) {
}
/* fzgx:end fn_8_5C50 */

/* fzgx:begin fn_8_6558 */
// fn_8_6558: returns a constant.
int fn_8_6558(void) {
    return 0;
}
/* fzgx:end fn_8_6558 */

/* fzgx:begin fn_8_6560 */
// fn_8_6560: returns a constant.
int fn_8_6560(void) {
    return 0;
}
/* fzgx:end fn_8_6560 */

/* fzgx:begin fn_8_6568 */
// fn_8_6568: returns a constant.
int fn_8_6568(void) {
    return 2;
}
/* fzgx:end fn_8_6568 */

/* fzgx:begin fn_8_6570 */
extern u32 fn_1_D0790(void);

s32 fn_8_6570(void) {
    fn_1_D0790();
    return 0;
}
/* fzgx:end fn_8_6570 */

/* fzgx:begin fn_8_67B0 */
extern u32 fn_1_3F8C0(void);
extern u32 fn_1_48140(u32);
extern u32 fn_80008E84(u32);
extern u32 fn_8_8C80(u32);

void fn_8_67B0(void) {
    u32 t0, t1, t3;
    t0 = fn_1_3F8C0();
    t1 = fn_80008E84(t0);
    fn_1_48140(148);
    t3 = fn_80008E84(t1);
    fn_8_8C80(t3);
}
/* fzgx:end fn_8_67B0 */
