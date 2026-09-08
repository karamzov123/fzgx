#include "types.h"

/* fzgx:begin fn_1_D1780 */
#include "rel/main_rel/face.h"

extern u32 lbl_1_data_3D924;
extern u8 lbl_1_data_3DA2C[0xD4];
extern u32 lbl_1_bss_7ACF0;

extern u32 lbl_801A6410;
extern u32 lbl_1_bss_7ACEC;
extern u8 lbl_1_data_3DA2C[212];
extern u32 fn_1_45D0(u32, u32, void*, u32);
extern u32 fn_80008F88(void*, u32);

void fn_1_D1780(void) {
    u32 result;

    lbl_1_bss_7ACEC = 0x40000;
    result = fn_1_45D0(lbl_801A6410, 0x40000, lbl_1_data_3DA2C, 0x14a);
    lbl_1_bss_7ACF0 = result;
    lbl_1_data_3D924 = fn_80008F88((void*)result, result + lbl_1_bss_7ACEC);
}
/* fzgx:end fn_1_D1780 */

/* fzgx:begin fn_1_D17E8 */
extern u32 lbl_1_data_3D924;
extern u32 lbl_1_bss_7ACF0;
extern u32 lbl_801A6410;
extern u8 lbl_1_data_3DA2C[212];

extern void fn_80009064(u32);
extern void fn_1_46B4(u32, u32, u8*, int);

void fn_1_D17E8(void) {
    fn_80009064(lbl_1_data_3D924);
    fn_1_46B4(lbl_801A6410, lbl_1_bss_7ACF0, lbl_1_data_3DA2C, 0x152);
    lbl_1_data_3D924 = -1;
}
/* fzgx:end fn_1_D17E8 */

/* fzgx:begin fn_1_D1F88 noprologue */
#include "types.h"

typedef struct {
    u32 unk_0;  // 8 loads, 8 stores
    u32 unk_4;  // 13 loads, 4 stores
    u32 unk_8;  // 0 loads, 3 stores
    u8 pad_C[0x4];
    u32 unk_10;  // 7 loads, 4 stores
    u32 unk_14;  // 7 loads, 4 stores
    u32 unk_18;  // 0 loads, 6 stores
    u32 unk_1C;  // 2 loads, 5 stores
    u8 pad_20[0xC];
    u32 unk_2C;  // 0 loads, 1 stores
    u32 unk_30;  // 0 loads, 1 stores
    u32 unk_34;  // 0 loads, 1 stores
    u32 unk_38;  // 3 loads, 5 stores
    u32 unk_3C;  // 1 loads, 8 stores
    u8 pad_40[0x20];
    u32 unk_60;  // 4 loads, 2 stores
} Obj_1_bss_7ACD8_Target;
extern Obj_1_bss_7ACD8_Target *lbl_1_bss_7ACD8;
extern u32 lbl_1_data_3D924;
extern u8 lbl_1_data_3DA2C[0xD4];

extern void fn_1_D2F50(void);
extern void fn_1_46B4(u32, u32, u8*, s32);
extern void fn_80071718(u32);
extern void fn_800711A8(u32);
extern void fn_1_D2F84(void);
extern u32 lbl_801A6410;
extern u8 lbl_1_data_3DA2C[212];

void fn_1_D1F88(void) {
    Obj_1_bss_7ACD8_Target* state;
    s32 i;
    Obj_1_bss_7ACD8_Target* obj;
    u32 index;

    state = (Obj_1_bss_7ACD8_Target*)&lbl_1_bss_7ACD8;
    if ((s32)lbl_1_data_3D924 != -1) {
        fn_1_D2F50();
        i = 0;
        obj = (Obj_1_bss_7ACD8_Target*)state->unk_0;
        for (; i < 6; i++, obj = (Obj_1_bss_7ACD8_Target*)((u8*)obj + 0x64)) {
            if (obj->unk_60 != 0) {
                fn_1_46B4(lbl_801A6410, obj->unk_60, lbl_1_data_3DA2C, 0x271);
                obj->unk_60 = 0;
            }
            if (i < *(u8*)((u8*)state + 0xD)) {
                index = (obj->unk_4 & 0xFF) << 2;
                if (*(u32*)(state->unk_8 + index) != 0) {
                    fn_80071718(*(u32*)(state->unk_8 + index));
                    *(u32*)(state->unk_8 + index) = 0;
                }
                if (*(u32*)(state->unk_4 + index) != 0) {
                    fn_800711A8(*(u32*)(state->unk_4 + index));
                    *(u32*)(state->unk_4 + index) = 0;
                }
            }
        }
        if (state->unk_4 != 0) {
            fn_1_46B4(lbl_801A6410, state->unk_4, lbl_1_data_3DA2C, 0x277);
            state->unk_4 = 0;
        }
        if (state->unk_8 != 0) {
            fn_1_46B4(lbl_801A6410, state->unk_8, lbl_1_data_3DA2C, 0x278);
            state->unk_8 = 0;
        }
        if (state->unk_0 != 0) {
            fn_1_46B4(lbl_801A6410, state->unk_0, lbl_1_data_3DA2C, 0x27B);
            state->unk_0 = 0;
        }
        fn_1_D2F84();
    }
}
/* fzgx:end fn_1_D1F88 */

/* fzgx:begin fn_1_D2D24 */
#include "rel/main_rel/face.h"

Obj_1_bss_7ACD8_Target *fn_1_D2D24(s32 index) {
    return &lbl_1_bss_7ACD8[index];
}
/* fzgx:end fn_1_D2D24 */

/* fzgx:begin fn_1_D2D38 */
extern u32 lbl_1_bss_7ACE8;

u32 fn_1_D2D38(void) {
    return lbl_1_bss_7ACE8;
}
/* fzgx:end fn_1_D2D38 */

/* fzgx:begin fn_1_D2D48 */
extern u8 lbl_1_bss_7ACE5;

u8 fn_1_D2D48(void) {
    return lbl_1_bss_7ACE5;
}
/* fzgx:end fn_1_D2D48 */

/* fzgx:begin fn_1_D2D58 */
#include "rel/main_rel/face.h"

extern s16 fn_1_3F0C8(void *obj);
extern u32 fn_1_864E8(u32 value);

Obj_1_bss_7ACD8_Target *fn_1_D2D58(void *obj) {
    s32 i;

    for (i = 0; i < lbl_1_bss_7ACE5; i++) {
        if ((u32)obj == lbl_1_bss_7ACD8[i].unk_10) {
            if (fn_1_3F0C8(obj) == 0x29 &&
                (fn_1_864E8(lbl_1_bss_7ACD8[i].unk_4) & 0x20000880) != 0) {
                return 0;
            }
            return &lbl_1_bss_7ACD8[i];
        }
    }
    return 0;
}
/* fzgx:end fn_1_D2D58 */

/* fzgx:begin fn_1_D2E18 */
// fn_1_D2E18: 3-way byte comparison (a > b returns 1, a < b returns -1, a == b returns 0)
s32 fn_1_D2E18(u8* a, u8* b) {
    u8 va = *a;
    u8 vb = *b;
    s32 d1;
    s32 d2;
    if (va > vb) {
        return 1;
    }
    d1 = va - vb;
    d2 = vb - va;
    return (d1 | d2) >> 31;
}
/* fzgx:end fn_1_D2E18 */

/* fzgx:begin fn_1_D2E44 */
#include "rel/main_rel/face.h"

extern void fn_8000C49C(char*, ...);
extern u32 fn_1_86810(u32);
extern void fn_8008279C(void*, u32, u32, void*);
extern s32 fn_1_D2E18(u8*, u8*);

s32 fn_1_D2E44(u32 index) {
    u8 values[4];
    u32 j;
    u32 i;

    if ((s32)lbl_1_bss_7ACE8 != 0) {
        if (index >= 4) {
            fn_8000C49C((char*)lbl_1_data_3DA2C, 0x4f3, (char*)lbl_1_data_3DB2C);
        }
        for (i = 0; (u8)i < lbl_1_bss_7ACE5; i++) {
            values[(u8)i] = fn_1_86810((u8)i);
        }
        fn_8008279C(values, lbl_1_bss_7ACE5, 1, fn_1_D2E18);
        for (j = 0; (u8)j < lbl_1_bss_7ACE5; j++) {
            if (values[(u8)j] == (u8)fn_1_86810(index)) {
                return j;
            }
        }
        return values[index];
    }
    return fn_1_86810(index);
}
/* fzgx:end fn_1_D2E44 */

/* fzgx:begin fn_1_D2FB0 */
#include "rel/main_rel/face.h"

u32 fn_1_D2FB0(void) {
    return lbl_1_data_3D924;
}
/* fzgx:end fn_1_D2FB0 */

/* fzgx:begin fn_1_D3144 */
extern u32 lbl_1_bss_7AD00;

// fn_1_D3144: Conditionally call function pointer from BSS
void fn_1_D3144(void) {
    u32 fn_ptr = lbl_1_bss_7AD00;
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
/* fzgx:end fn_1_D3144 */

/* fzgx:begin fn_1_D3178 */
extern u32 lbl_1_bss_7AD04;

void fn_1_D3178(void) {
    u32 fn_ptr = lbl_1_bss_7AD04;
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
/* fzgx:end fn_1_D3178 */

/* fzgx:begin fn_1_D31AC */
extern u32 lbl_1_bss_7AD08[7];

void fn_1_D31AC(void) {
    u32 fn_ptr = lbl_1_bss_7AD08[0];
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
/* fzgx:end fn_1_D31AC */

/* fzgx:begin fn_1_D31E0 */
extern u32 lbl_1_bss_7AD24[9];

void fn_1_D31E0(void) {
    u32 fn_ptr = lbl_1_bss_7AD24[0];
    if (fn_ptr != 0) {
        ((void (*)(void))fn_ptr)();
    }
}
/* fzgx:end fn_1_D31E0 */
