#include "types.h"

/* fzgx:begin fn_1_E5724 */
s16 fn_1_E5724(void *base) {
    return *(s16 *)((char *)base + 0x8);
}
/* fzgx:end fn_1_E5724 */

/* fzgx:begin fn_1_E572C */
s16 fn_1_E572C(void *base) {
    return *(s16 *)((char *)base + 0xA);
}
/* fzgx:end fn_1_E572C */

/* fzgx:begin fn_1_E5734 */
typedef struct {
    u32 unk0;
    f32 value;
} E5734Obj;

// Returns the float at offset 4 of the object.
f32 fn_1_E5734(E5734Obj *self) {
    return self->value;
}
/* fzgx:end fn_1_E5734 */

/* fzgx:begin fn_1_E5748 */
// Returns a pointer to the member at offset 0x54.
u8 *fn_1_E5748(u8 *self) {
    return self + 0x54;
}
/* fzgx:end fn_1_E5748 */

/* fzgx:begin fn_1_E5750 */
// Returns a pointer to the member at offset 0x44.
u8 *fn_1_E5750(u8 *self) {
    return self + 0x44;
}
/* fzgx:end fn_1_E5750 */

/* fzgx:begin fn_1_E5758 */
f32 fn_1_E5758(u8 *self) {
    return *(f32 *)(self + 0x50);
}
/* fzgx:end fn_1_E5758 */

/* fzgx:begin fn_1_E57A4 */
f32 fn_1_E57A4(void *base) {
    return *(f32 *)((char *)base + 0x18);
}
/* fzgx:end fn_1_E57A4 */

/* fzgx:begin fn_1_E57AC */
f32 fn_1_E57AC(void *base) {
    return *(f32 *)((char *)base + 0x1c);
}
/* fzgx:end fn_1_E57AC */

/* fzgx:begin fn_1_E57B4 */
f32 fn_1_E57B4(void *base) {
    return *(f32 *)((char *)base + 0x20);
}
/* fzgx:end fn_1_E57B4 */

/* fzgx:begin fn_1_E57F4 */
void fn_1_E57F4(void *base, f32 value) {
    *(f32 *)((char *)base + 0x4) = value;
}
/* fzgx:end fn_1_E57F4 */

/* fzgx:begin fn_1_E5884 */
extern f32 lbl_1_rodata_67A8;
extern void fn_80008BA8(void *dst, const void *src, u32 size);

void fn_1_E5884(void *base, const void *value) {
    if (value == 0) {
        f32 zero = lbl_1_rodata_67A8;
        *(f32 *)((char *)base + 0xfc) = zero;
        *(f32 *)((char *)base + 0xf8) = zero;
        *(f32 *)((char *)base + 0xf4) = zero;
    } else {
        fn_80008BA8((char *)base + 0xf4, value, 0xc);
    }
}
/* fzgx:end fn_1_E5884 */

/* fzgx:begin fn_1_E5980 */
// fn_1_E5980: Store float value at offset 0x20 in base pointer
void fn_1_E5980(void *base, f32 value) {
    *(f32 *)((char *)base + 0x20) = value;
}
/* fzgx:end fn_1_E5980 */

/* fzgx:begin fn_1_E5A50 */
extern void fn_80008BEC(void *dst, void *value, u32 size);
extern void fn_80008BA8(void *dst, void *value, u32 size);

void fn_1_E5A50(void *base, void *value) {
    if (value == NULL) {
        fn_80008BEC((void *)((char *)base + 0x38), NULL, 0xC);
    } else {
        fn_80008BA8((void *)((char *)base + 0x38), value, 0xC);
    }
}
/* fzgx:end fn_1_E5A50 */

/* fzgx:begin fn_1_E9C68 */
extern u32 lbl_1_bss_7B188[2];
extern s16 lbl_1_bss_960;
extern u32 fn_1_3F8C0(void);
extern u8 lbl_1_bss_7C85C[113];
extern u32 lbl_1_data_3E52C;
extern u8 lbl_1_data_3E434[68];
extern u32 fn_80008E84(u32 value);

void fn_1_E9C68(void) {
    u32 value;

    if ((s32)lbl_1_bss_7B188[0] == 0) {
        return;
    }

    if (lbl_1_bss_960 == 9) {
        value = fn_80008E84(fn_1_3F8C0());
        *(u32*)lbl_1_bss_7C85C = value;
    }

    if ((s32)lbl_1_data_3E52C >= 0) {
        ((void (**)(void))lbl_1_data_3E434)[lbl_1_data_3E52C]();
    }

    if (lbl_1_bss_960 == 9) {
        fn_80008E84(*(u32*)lbl_1_bss_7C85C);
    }
}
/* fzgx:end fn_1_E9C68 */

/* fzgx:begin fn_1_EB080 */
extern u32 lbl_1_bss_7B188[2];
extern u32 lbl_1_data_3E358[3];

// fn_1_EB080: Check BSS flag, conditionally copy data field, store parameter.
void fn_1_EB080(int param) {
    u8* data_ptr = (u8*)lbl_1_data_3E358;
    
    if ((int)lbl_1_bss_7B188[0] == 0) {
        return;
    }
    
    if (param == 11) {
        *(u32*)(data_ptr + 0x1e0) = *(u32*)(data_ptr + 0x1d4);
    }
    
    *(u32*)(data_ptr + 0x1d8) = param;
}
/* fzgx:end fn_1_EB080 */

/* fzgx:begin fn_1_EB200 */
extern u32 lbl_1_bss_7B188[2];
extern s32 lbl_1_data_3E52C;

int fn_1_EB200(void) {
    if ((s32)lbl_1_bss_7B188[0] == 0) {
        return 0;
    }

    if (lbl_1_data_3E52C == 14 ||
        lbl_1_data_3E52C == 16 ||
        lbl_1_data_3E52C == 15) {
        return 1;
    }

    return 0;
}
/* fzgx:end fn_1_EB200 */

/* fzgx:begin fn_1_EF924 */
extern void fn_1_EE530(void);
extern u32 lbl_1_data_3E538;
extern u32 lbl_1_data_3E52C;
extern u8 lbl_1_bss_7ECB4[72];
extern void fn_1_BC29C(u8 *);
extern void fn_1_B9C0C(void);
extern void fn_1_1596DC(s32);
extern void fn_1_484CC(s32);

void fn_1_EF924(void) {
    fn_1_EE530();
    lbl_1_data_3E52C = lbl_1_data_3E538;
    fn_1_BC29C(lbl_1_bss_7ECB4);
    fn_1_B9C0C();
    fn_1_1596DC(2);
    fn_1_484CC(2);
}
/* fzgx:end fn_1_EF924 */
