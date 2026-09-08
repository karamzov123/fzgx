#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/alloc.h"

/* fzgx:begin fn_1_4928 */
#include "rel/main_rel/alloc.h"

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} Fn1_4928Entry;

extern Fn1_4928Entry lbl_1_bss_DCC[32];
extern char lbl_1_data_2C9C[20];
extern void fn_8000C49C(const char *message, int line, const char *file, ...);
extern void fn_80009AA8(u32 value, u32 count, const char *file, int line);

void fn_1_4928(void) {
    u32 value;
    u32 count;
    s32 remaining;
    Fn1_4928Entry *entry;

    entry = lbl_1_bss_DCC;
    remaining = 0x20;
    while (remaining > 0) {
        if (entry->unk_08 != 0) {
            entry->unk_04--;
            if (entry->unk_04 == 0) {
                count = entry->unk_08;
                value = entry->unk_00;
                if (count == 0) {
                    fn_8000C49C((const char *)&lbl_1_data_2CD0, 0x15b,
                                lbl_1_data_2C9C);
                }
                fn_80009AA8(value, count, (const char *)&lbl_1_data_2CD0,
                            0x15b);
                entry->unk_08 = 0;
            }
        }
        remaining--;
        entry++;
    }
}
/* fzgx:end fn_1_4928 */

/* fzgx:begin fn_1_49F0 */
void fn_1_49F0(u32 *value) {
    lbl_1_data_2CD8 = *value;
}
/* fzgx:end fn_1_49F0 */

/* fzgx:begin fn_1_4BB0 */
#include "rel/main_rel/alloc.h"

extern u32 fn_1_435C(u32 value);
extern void fn_1_426C(s16 value);

void fn_1_4BB0(void) {
    if (lbl_1_data_2CDC.unk_3 & 1) {
        u32 value = fn_1_435C(lbl_1_data_2CDC.unk_C);
        fn_1_426C(lbl_1_data_2CDC.unk_6);
        fn_1_435C(value);
    }
}
/* fzgx:end fn_1_4BB0 */

/* fzgx:begin fn_1_4C10 */
#include "rel/main_rel/alloc.h"

int fn_1_4C10(void) {
    u8 value = lbl_1_data_2CDC.unk_3;
    return value & 1;
}
/* fzgx:end fn_1_4C10 */

/* fzgx:begin fn_1_4CAC */
#include "rel/main_rel/alloc.h"

u32 fn_1_4CAC(void) {
    return (lbl_1_data_2CDC.unk_3 >> 1) & 1;
}
/* fzgx:end fn_1_4CAC */

/* fzgx:begin fn_1_4CC0 */
#include "rel/main_rel/alloc.h"

void fn_1_4CC0(void) {
    lbl_1_data_2CDC.unk_3 &= 0xfe;
}
/* fzgx:end fn_1_4CC0 */

/* fzgx:begin fn_1_4D14 pool */
extern f32 lbl_1_rodata_160;
extern f32 lbl_1_rodata_164[2];

typedef struct Fn14D14Data {
    u8 value0;
    u8 value1;
    u8 unk2;
    u8 flags;
    u16 value4;
} Fn14D14Data;

f32 fn_1_4D14(Fn14D14Data *data) {
    f32 value3;
    u8 value;
    f32 value1;
    f32 temp;

    value3 = ((f32)data->value4) / lbl_1_rodata_160;
    value = 0x0f;
    if (data->value0 != 0x3c) {
        value = data->value0;
    }
    if (data->value1 > value) {
        value1 = lbl_1_rodata_164[0];
    } else {
        value1 = ((f32)data->value1) / (f32)value;
    }

    if (data->flags & 2) {
        value1 = value3 * (lbl_1_rodata_164[0] - value1);
    } else {
        temp = lbl_1_rodata_164[0] - value3;
        temp = temp * value1;
        value1 = value3 + temp;
    }
    return value1;
}
/* fzgx:end fn_1_4D14 */

/* fzgx:begin fn_1_4FEC */
void fn_1_4FEC(void) {
    lbl_1_bss_F58 = 0;
}
/* fzgx:end fn_1_4FEC */

/* fzgx:begin fn_1_509C noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"

extern u32 lbl_1_bss_F58;
extern u32 lbl_1_bss_7AC40[2];
extern u32 lbl_1_bss_7AC20[8];

s32 fn_8008023C(u32 *, u32 *, u32);

s32 fn_1_509C(void) {
    lbl_1_bss_7AC40[0] = lbl_1_bss_F58;
    if (fn_8008023C(lbl_1_bss_7AC20, lbl_1_bss_7AC40, 4) == 0) {
        return 0;
    }
    lbl_1_bss_7AC20[0] = lbl_1_bss_7AC40[0];
    return 1;
}
/* fzgx:end fn_1_509C */

/* fzgx:begin fn_1_5104 */
#include "rel/main_rel/alloc.h"

void fn_1_5104(void) {
    lbl_1_bss_F58 = lbl_1_bss_7AC40;
    lbl_1_bss_7AC20.unk_0 = lbl_1_bss_7AC40;
}
/* fzgx:end fn_1_5104 */

/* fzgx:begin fn_1_5124 */
#include "rel/main_rel/alloc.h"

typedef struct {
    Obj_1_bss_F68_Target *unk_0;
    Obj_1_bss_F68_Target *unk_4;
    Obj_1_bss_F68_Target *unk_8;
    u8 pad_C[2];
    u8 unk_E;
} InitState;

extern u32 lbl_801A6410;
extern void fn_8000C49C(const char *, ...);
extern Obj_1_bss_F68_Target *fn_1_45D0(u32, u32, u8 *, u32);
extern void fn_800793D4(Obj_1_bss_F68_Target *, u32, u32);
extern void fn_1_435C(u32);
extern u16 fn_1_3F8C(u8 *, void (*)(void), u32, u32);
extern void fn_1_6394(void);
extern void fn_1_63D4(void);
extern void fn_1_DF74(void);

void fn_1_5124(u32 arg0, u32 arg1) {
    InitState *state = (InitState *)&lbl_1_bss_F68;
    u8 *data = lbl_1_data_32A8;
    Obj_1_bss_F68_Target *allocated;

    if (state->unk_0 != 0) {
        fn_8000C49C((const char *)(data + 0x70), 0x69, (const char *)(data + 0x7c));
    }
    state->unk_0 = fn_1_45D0(lbl_801A6410, 0x78, data + 0x70, 0x6b);
    fn_800793D4(state->unk_0, 0, 0x78);

    if (state->unk_4 != 0) {
        fn_8000C49C((const char *)(data + 0x70), 0x70, (const char *)(data + 0xac));
    }
    state->unk_4 = fn_1_45D0(lbl_801A6410, 0x7f0, data + 0x70, 0x72);
    fn_800793D4(state->unk_4, 0, 0x7f0);

    if (state->unk_8 != 0) {
        fn_8000C49C((const char *)(data + 0x70), 0x78, (const char *)(data + 0xdc));
    }
    allocated = fn_1_45D0(lbl_801A6410, 0x1e0, data + 0x70, 0x7a);
    state->unk_8 = allocated;
    fn_800793D4(allocated, 0, 0x1e0);

    state->unk_0->unk_50 = arg0;
    state->unk_0->unk_54 = arg1;
    state->unk_0->unk_4B = state->unk_E;
    state->unk_0->unk_4C = data[0x6c];

    fn_1_435C(state->unk_0->unk_50);
    state->unk_0->unk_24 = fn_1_3F8C(data + 0x10c, fn_1_6394, 0, 0x15);
    fn_1_435C(state->unk_0->unk_54);
    state->unk_0->unk_46 = fn_1_3F8C(data + 0x118, fn_1_63D4, 0, 0x18);
    fn_1_DF74();
}
/* fzgx:end fn_1_5124 */

/* fzgx:begin fn_1_52E0 noprologue */
#include "types.h"

typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn152E0State;

extern Fn152E0State *lbl_1_bss_F68;
extern u8 lbl_1_bss_F76;

void fn_1_52E0(u8 value) {
    lbl_1_bss_F76 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4b = value;
    }
}
/* fzgx:end fn_1_52E0 */

/* fzgx:begin fn_1_5300 noprologue */
#include "types.h"

typedef struct {
    u8 _pad[0x4b];
    u8 field_4b;
} Fn15300State;

extern Fn15300State *lbl_1_bss_F68;

u8 fn_1_5300(void) {
    u32 value;

    if (lbl_1_bss_F68 != 0) {
        value = lbl_1_bss_F68->field_4b;
    } else {
        value = 0;
    }
    return (u8)value;
}
/* fzgx:end fn_1_5300 */

/* fzgx:begin fn_1_5328 noprologue */
#include "types.h"

typedef struct {
    u8 _pad[0x4c];
    u8 field_4c;
} Fn15328State;

extern Fn15328State *lbl_1_bss_F68;
extern u8 lbl_1_data_3314;

void fn_1_5328(u8 value) {
    lbl_1_data_3314 = value;
    if (lbl_1_bss_F68 != 0) {
        lbl_1_bss_F68->field_4c = value;
    }
}
/* fzgx:end fn_1_5328 */

/* fzgx:begin fn_1_5348 noprologue */
#include "types.h"

typedef struct {
    u8 _pad[0x4c];
    u8 field_4c;
} Fn15348State;

extern Fn15348State *lbl_1_bss_F68;

u8 fn_1_5348(void) {
    u32 value;

    if (lbl_1_bss_F68 != 0) {
        value = lbl_1_bss_F68->field_4c;
    } else {
        value = 0;
    }
    return (u8)value;
}
/* fzgx:end fn_1_5348 */

/* fzgx:begin fn_1_58C4 */
#include "rel/main_rel/alloc.h"

u8 fn_1_58C4(void) {
    Obj_1_bss_F68_Target *obj = lbl_1_bss_F68;

    if (obj == 0) {
        return 0;
    }
    if ((s8)obj->unk_48 == 0xB) {
        return 1;
    }
    if ((obj->unk_0 & ((u32)1 << 31)) != 0) {
        return 1;
    }
    return obj->unk_49;
}
/* fzgx:end fn_1_58C4 */

/* fzgx:begin fn_1_5910 */
#include "rel/main_rel/alloc.h"

u8 fn_1_5910(void) {
    Obj_1_bss_F68_Target *obj;

    obj = lbl_1_bss_F68;
    if (obj == 0) {
        return 0;
    }
    if ((obj->unk_0 & (1u << 31)) != 0) {
        return 0;
    }
    return obj->unk_4A;
}
/* fzgx:end fn_1_5910 */

/* fzgx:begin fn_1_6394 */
#include "rel/main_rel/alloc.h"

extern void fn_1_DA6C(LiveCamera *);
extern void fn_1_DE14(LiveCamera *);
extern void fn_1_E174(void);

void fn_1_6394(void) {
    fn_1_DA6C(live_camera);
    fn_1_DE14(live_camera);
    fn_1_E174();
}
/* fzgx:end fn_1_6394 */

/* fzgx:begin fn_1_63D4 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"

extern u32 live_camera;
void fn_1_F3D0(u32);

void fn_1_63D4(void) {
    fn_1_F3D0(live_camera);
}
/* fzgx:end fn_1_63D4 */
