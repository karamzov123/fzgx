#include "types.h"

/* fzgx:begin fn_1_151AF0 */
extern int fn_8003BE1C(void *arg0, void *arg1);
extern int fn_8003C1A4(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4);
extern void fn_8003CE74(void *arg0, void *arg1, int arg2);
extern void fn_8003CBB0(void *arg0, void *arg1, void *arg2, void *arg3);
extern void fn_8003C6D0(void *arg0, void *arg1, void *arg2, void *arg3);

int fn_1_151AF0(void *arg0, int mode, void *arg2, void *arg3, void *arg4, void *arg5) {
    u8 local_8[4];
    u8 local_c[8];
    u8 local_14[12];

    if (fn_8003BE1C(arg0, local_c) == 0) {
        return 0;
    }
    if (fn_8003C1A4(local_14, local_8, arg5, arg0, local_c) == 0) {
        return 0;
    }
    switch (mode) {
    case 0:
        fn_8003CE74(local_14, arg2, 0xff);
        break;
    case 1:
        fn_8003CBB0(local_14, arg2, arg3, arg4);
        break;
    case 2:
        fn_8003C6D0(local_14, arg2, arg3, arg4);
        break;
    }
    return 1;
}
/* fzgx:end fn_1_151AF0 */

/* fzgx:begin fn_1_151BDC */
extern u32 lbl_1_bss_8ED88;

void fn_1_151BDC(u32 value) {
    lbl_1_bss_8ED88 = value;
}
/* fzgx:end fn_1_151BDC */

/* fzgx:begin fn_1_151C3C */
extern u16 lbl_1_bss_8ED8C;
extern u16 lbl_1_bss_8ED8E;
extern void fn_1_151C9C(void);
extern void fn_80074A7C(void (*callback)(void));

void fn_1_151C3C(u16 value1, u16 value2) {
    lbl_1_bss_8ED8C = value1;
    lbl_1_bss_8ED8E = value2;
    fn_80074A7C(fn_1_151C9C);
}
/* fzgx:end fn_1_151C3C */

/* fzgx:begin fn_1_151C78 */
extern void fn_80074A7C(int value);

void fn_1_151C78(void) {
    fn_80074A7C(0);
}
/* fzgx:end fn_1_151C78 */

/* fzgx:begin fn_1_151E20 */
u32 fn_1_151E20(u32 value, s16 index) {
    u32 result = (u32)-1;

    switch (index) {
    case 0:
        result = value >> 24;
        break;
    case 1:
        result = (value >> 16) & 0xff;
        break;
    case 2:
        result = (value >> 8) & 0xff;
        break;
    case 3:
        result = value & 0xff;
        break;
    }

    return result;
}
/* fzgx:end fn_1_151E20 */

/* fzgx:begin fn_1_151E74 */
void fn_1_151E74(u32 *value, s16 index, int byte) {
    switch (index) {
    case 0:
        *value = (*value & 0x00FFFFFF) | ((u32)byte << 24);
        break;
    case 1:
        *value = (*value & 0xFF00FFFF) | ((u32)byte << 16);
        break;
    case 2:
        *value = (*value & 0xFFFF00FF) | ((u32)byte << 8);
        break;
    case 3:
        *value = (*value & 0xFFFFFF00) | byte;
        break;
    default:
        break;
    }
}
/* fzgx:end fn_1_151E74 */

/* fzgx:begin fn_1_152840 */
typedef struct Entry {
    s16 id;
    u8 pad[2];
    u32 flags;
} Entry;

int fn_1_152840(s16 id, Entry *entries, s16 outer_count, s16 inner_count) {
    s16 count;
    s16 i;
    s16 j;

    if (id == -1) {
        return 0;
    }

    count = inner_count;
    for (i = 0; i < outer_count; i++) {
        for (j = 0; j < count; j++) {
            if ((entries[(i * count) + j].flags & (3u << 30)) == 0 &&
                id == entries[(i * count) + j].id) {
                return 1;
            }
        }
    }

    return 0;
}
/* fzgx:end fn_1_152840 */

/* fzgx:begin fn_1_153988 */
// fn_1_153988: empty in retail (single blr).
void fn_1_153988(void) {
}
/* fzgx:end fn_1_153988 */

/* fzgx:begin fn_1_15398C */
#include "rel/main_rel/bg_aur.h"

extern void fn_1_153AF8(Obj_1_data_2A7E0_At3C *);
extern void fn_1_153D48(Obj_1_data_2A7E0_At3C *);
extern void fn_1_9A508(void);

void fn_1_15398C(void) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    fn_1_153AF8(obj);
    fn_1_153D48(obj);
    fn_1_9A508();
}
/* fzgx:end fn_1_15398C */

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

/* fzgx:begin fn_1_153A28 */
#include "rel/main_rel/bg_aur.h"

extern void fn_1_9D260(void);
extern void fn_1_9AD54(void);
extern int fn_1_58C4(void);
extern void fn_1_5948(int);
extern void fn_1_153D74(void *);
extern void fn_1_627C(int);
extern void fn_1_153B24(void *);

void fn_1_153A28(void) {
    Obj_1_data_2A7E0_At3C *obj;
    int count;
    int i;

    obj = lbl_1_data_2A7E0.unk_3C;
    fn_1_9D260();
    fn_1_9AD54();
    count = fn_1_58C4();
    for (i = 0; i < count; i++) {
        fn_1_5948(i);
        fn_1_153D74(obj);
        fn_1_627C(i);
    }
    fn_1_153B24(obj);
}
/* fzgx:end fn_1_153A28 */

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

/* fzgx:begin fn_1_153AF8 */
void fn_1_153AF8(u32 *p) {
    u32 *q = p;
    int i = 0;

    p[0] = 0;
    while (i < (int)p[0]) {
        *++q = 0;
        i++;
    }
}
/* fzgx:end fn_1_153AF8 */

/* fzgx:begin fn_1_153D48 */
extern void fn_80008BEC(void *arg0, u32 arg1, u32 arg2);

void fn_1_153D48(void *arg0) {
    fn_80008BEC((u8 *)arg0 + 0x1108, 0, 0x5fc);
}
/* fzgx:end fn_1_153D48 */

/* fzgx:begin fn_1_1543E8 */
#include "rel/main_rel/bg_aur.h"

u32 fn_1_1543E8(s32 value, u32 *source) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    switch (value) {
    case 0:
        obj->unk_1104 = *source;
        break;
    default:
        break;
    }

    return 1;
}
/* fzgx:end fn_1_1543E8 */
