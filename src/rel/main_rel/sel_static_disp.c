#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_133DBC */
extern void fn_1_133E04(void* arg0, s32 arg1);

void fn_1_133DBC(void* arg0) {
    fn_1_133E04(arg0, 0);
}
/* fzgx:end fn_1_133DBC */

/* fzgx:begin fn_1_133DE0 */
extern void fn_1_133E04(void* arg0, s32 arg1);

void fn_1_133DE0(void* arg0) {
    fn_1_133E04(arg0, 1);
}
/* fzgx:end fn_1_133DE0 */

/* fzgx:begin fn_1_134A4C */
typedef struct StaticDispParams {
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    u32 unk1[8];
    s32 count;
    u32 unk2[9];
} StaticDispParams;

extern StaticDispParams lbl_1_rodata_26F8;
extern const f32 lbl_1_rodata_882C;
extern const f32 lbl_1_rodata_8830;
extern const f32 lbl_1_rodata_8834;
extern void fn_1_4F734(void* arg0);

void fn_1_134A4C(void) {
    StaticDispParams params;

    params = lbl_1_rodata_26F8;
    params.id = 0x9a14;
    params.x = lbl_1_rodata_882C;
    params.y = lbl_1_rodata_8830;
    params.z = lbl_1_rodata_8834;
    params.count = 10;
    fn_1_4F734(&params.id);
}
/* fzgx:end fn_1_134A4C */

/* fzgx:begin fn_1_135894 */
extern const f32 lbl_1_rodata_87DC;
extern void fn_1_135D7C(void* arg0, void* arg1, void* arg2, void* arg3,
                         void* arg4, void* arg5, void* arg6, f32 arg7,
                         f32 arg8, f32 arg9);

void fn_1_135894(void* arg0, void* arg1, void* arg2, void* arg3,
                 void* arg4, void* arg5, void* arg6, f32 arg7, f32 arg8) {
    fn_1_135D7C(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8,
                lbl_1_rodata_87DC);
}
/* fzgx:end fn_1_135894 */

/* fzgx:begin fn_1_137288 */
extern void* fn_1_12F118(void);
extern void* fn_1_36AD0(void);
extern void fn_1_1373B0(int index, void* value0, void* value1, void* value2, s16 selector);
extern void fn_1_137364(int index, void* value0, void* value1, void* value2, s16 selector);

// Selects the display data source and emits either its configured value or the fallback value.
void fn_1_137288(int index, void* value0, void* value1, void* value2, s16 selector) {
    void* entry;

    if ((s16)index >= 0x29) {
        entry = fn_1_12F118();
        if (entry == fn_1_36AD0()) {
            entry = (char*)entry + (s32)selector * 0x81c0;
        } else {
            entry = (char*)entry + (s32)((s16)index - 0x29) * 0x81c0;
        }

        if (*(u32*)entry & 0x40000000) {
            fn_1_1373B0(index, value0, value1, value2, selector);
        } else {
            fn_1_137364(*(u8*)((char*)entry + 0x81a0), value0, value1, value2, selector);
        }
    } else {
        fn_1_137364(index, value0, value1, value2, selector);
    }
}
/* fzgx:end fn_1_137288 */

/* fzgx:begin fn_1_13750C */
extern void fn_1_1375C0(int arg0, void* arg1, void* arg2, void* arg3, void* arg4);

void fn_1_13750C(void* arg0, void* arg1, void* arg2, void* arg3) {
    fn_1_1375C0(0, arg0, arg1, arg2, arg3);
}
/* fzgx:end fn_1_13750C */

/* fzgx:begin fn_1_137548 */
extern void fn_1_1375C0(int arg0, void* arg1, void* arg2, void* arg3, void* arg4);

void fn_1_137548(void* arg0, void* arg1, void* arg2, void* arg3) {
    fn_1_1375C0(1, arg0, arg1, arg2, arg3);
}
/* fzgx:end fn_1_137548 */

/* fzgx:begin fn_1_137584 */
extern void fn_1_1375C0(int arg0, void* arg1, void* arg2, void* arg3, void* arg4);

void fn_1_137584(void* arg0, void* arg1, void* arg2, void* arg3) {
    fn_1_1375C0(2, arg0, arg1, arg2, arg3);
}
/* fzgx:end fn_1_137584 */

/* fzgx:begin fn_1_1380F0 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_138144(void* arg0, int arg1);

void fn_1_1380F0(void* arg0) {
    fn_1_138144(arg0, lbl_1_bss_8B3A0.unk_94 & 0x40000000);
}
/* fzgx:end fn_1_1380F0 */

/* fzgx:begin fn_1_138120 */
extern void fn_1_138144(void* arg0, int arg1);

void fn_1_138120(void* arg0) {
    fn_1_138144(arg0, 0);
}
/* fzgx:end fn_1_138120 */

/* fzgx:begin fn_1_139F18 */
#include "rel/main_rel/sel_static_disp.h"

typedef struct {
    u8 pad_0[0x44];
    f32 unk_44;
} Fn139F18Child;

typedef struct {
    u8 pad_0[0x24];
    Fn139F18Child* unk_24;
    u8 pad_28[0x10];
    f32 unk_38;
    f32 unk_3c;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4c;
} Fn139F18Obj;

typedef struct {
    u32 value[22];
} Fn139F18Data;

extern const f32 lbl_1_rodata_861C;
extern const f32 lbl_1_rodata_85F0;
extern const f32 lbl_1_rodata_8BB8;
extern Fn139F18Data lbl_1_rodata_26F8;
extern const f32 lbl_1_rodata_8BBC;
extern const f32 lbl_1_rodata_86F8;
extern const f32 lbl_1_rodata_8B84;
extern const f32 lbl_1_rodata_8638;
extern void fn_1_8CED0(void* arg0, int arg1, int arg2);
extern const f32 lbl_1_rodata_87D0;
extern void fn_1_131B90(void);
extern void fn_1_8D3F8(void* arg0, void* arg1, void* arg2, void* arg3);

void fn_1_139F18(Fn139F18Obj* self) {
    Fn139F18Data local;

    self->unk_38 = lbl_1_rodata_861C;
    self->unk_3c = lbl_1_rodata_85F0;
    self->unk_40 = lbl_1_rodata_8BB8;
    self->unk_44 = lbl_1_rodata_861C;
    self->unk_48 = lbl_1_rodata_85F0;
    self->unk_4c = lbl_1_rodata_861C;

    local = lbl_1_rodata_26F8;
    ((f32*)local.value)[1] = lbl_1_rodata_8BBC;
    ((f32*)local.value)[2] = lbl_1_rodata_86F8;
    ((f32*)local.value)[3] = lbl_1_rodata_8B84;
    local.value[12] = 10;

    if (lbl_1_bss_8CA28.unk_0 != 0) {
        ((f32*)local.value)[4] *= lbl_1_rodata_8638;
        ((f32*)local.value)[5] *= lbl_1_rodata_8638;
    }

    fn_1_8CED0(self->unk_24, 0x280, 0x18c);
    self->unk_24->unk_44 = lbl_1_rodata_87D0;
    fn_1_8D3F8(self->unk_24, &local, fn_1_131B90, self);
}
/* fzgx:end fn_1_139F18 */

/* fzgx:begin fn_1_13A848 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_49410(void);
extern void fn_1_494DC(int arg0);
extern void fn_1_495B0(u32 arg0);
extern void fn_1_5233C(void);
extern void fn_1_49738(void* arg0);
extern const f32 lbl_1_rodata_861C;
extern void fn_1_49748(f32 arg0);
extern const f32 lbl_1_rodata_87DC;
extern void fn_1_4954C(f32 arg0);
extern void fn_1_495C8(void* arg0);
extern const f32 lbl_1_rodata_863C;
extern void fn_1_4966C(f32 arg0, f32 arg1);
extern void fn_1_4955C(f32 arg0, f32 arg1);
extern const f32 lbl_1_rodata_85F0;
extern void fn_1_49590(f32 arg0);
extern const f64 lbl_1_rodata_8600;
extern void fn_1_496FC(f32 arg0, f32 arg1);
extern void fn_1_4A0D8(void* arg0);
extern s16 fn_1_12C930(void* arg0);

void fn_1_13A848(s16 arg0, s16 arg1, void* arg2, void* arg3, int arg4, f32 arg5, f32 arg6) {
    fn_1_49410();
    fn_1_494DC(8);
    fn_1_495B0(0x8001 << 16);
    fn_1_49738(&fn_1_5233C);
    fn_1_49748(lbl_1_rodata_861C);
    fn_1_4954C(lbl_1_rodata_87DC);
    fn_1_495C8(arg2);
    fn_1_4966C(lbl_1_rodata_863C, lbl_1_rodata_861C);
    fn_1_4955C(arg5, arg6);
    fn_1_49590(lbl_1_rodata_85F0);
    fn_1_496FC((f32)arg0, (f32)arg1);
    if (arg4 == 1) {
        fn_1_4A0D8(&lbl_1_data_41B1C);
    } else {
        fn_1_4A0D8((void*)((u32*)&lbl_1_data_212A8)[fn_1_12C930(arg3)]);
    }
}
/* fzgx:end fn_1_13A848 */

/* fzgx:begin fn_1_13A994 */
extern void fn_1_13A9B8(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, int arg6);

void fn_1_13A994(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5) {
    fn_1_13A9B8(arg0, arg1, arg2, arg3, arg4, arg5, -1);
}
/* fzgx:end fn_1_13A994 */

/* fzgx:begin fn_1_13C134 */
extern const f32 lbl_1_rodata_861C;
extern const f32 lbl_1_rodata_85EC;
extern void fn_1_5621C(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

void fn_1_13C134(void) {
    fn_1_5621C(lbl_1_rodata_861C, lbl_1_rodata_861C,
               lbl_1_rodata_861C, lbl_1_rodata_85EC);
}
/* fzgx:end fn_1_13C134 */

/* fzgx:begin fn_1_13C16C */
extern const f32 lbl_1_rodata_863C;
extern void fn_1_5621C(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

void fn_1_13C16C(void) {
    fn_1_5621C(lbl_1_rodata_863C, lbl_1_rodata_863C,
               lbl_1_rodata_863C, lbl_1_rodata_863C);
}
/* fzgx:end fn_1_13C16C */

/* fzgx:begin fn_1_13E054 */
extern void fn_1_13E080(void* arg0, void* arg1, void* arg2, int arg3);

void fn_1_13E054(void* arg0, void* arg1, void* arg2, int arg3) {
    u8* fixed = (u8*)0;
    fixed += 0xA204;
    fn_1_13E080(arg0, arg1, fixed, 31);
}
/* fzgx:end fn_1_13E054 */

/* fzgx:begin fn_1_13F81C */
extern u32 lbl_1_bss_8E3E4[8];

// Clear all cached display-resource handles before loading a new selection.
void fn_1_13F81C(void) {
    lbl_1_bss_8E3E4[0] = 0;
    lbl_1_bss_8E3E4[1] = 0;
    lbl_1_bss_8E3E4[2] = 0;
    lbl_1_bss_8E3E4[3] = 0;
    lbl_1_bss_8E3E4[4] = 0;
    lbl_1_bss_8E3E4[5] = 0;
    lbl_1_bss_8E3E4[6] = 0;
    lbl_1_bss_8E3E4[7] = 0;
}
/* fzgx:end fn_1_13F81C */

/* fzgx:begin fn_1_13F848 */
extern u32 lbl_801A6410;
extern char lbl_1_data_419E0[18];
extern u32 lbl_1_bss_8E3E4[8];
extern u32 fn_1_45D0(u32 arg0, int arg1, void* arg2, int arg3);
extern void fn_1_FC414(u32 arg0, int arg1);

// Loads a display resource and activates the handle assigned to the selected slot.
void fn_1_13F848(u32 arg0) {
    u32 slot = arg0 & 0xFF;

    lbl_1_bss_8E3E4[slot] =
        fn_1_45D0(lbl_801A6410, 0x10440, lbl_1_data_419E0, 0x14F9);
    fn_1_FC414(lbl_1_bss_8E3E4[slot], 1);
}
/* fzgx:end fn_1_13F848 */

/* fzgx:begin fn_1_13F8B0 */
extern u32 lbl_1_bss_8E3E4[8];

// Return the cached display handle for the key's low-byte slot.
u32 fn_1_13F8B0(u32 key) {
    return lbl_1_bss_8E3E4[key & 0xFF];
}
/* fzgx:end fn_1_13F8B0 */

/* fzgx:begin fn_1_13F948 */
extern u32 lbl_1_bss_8E404[8];

// Reset all static-display resource handles before initialization.
void fn_1_13F948(void) {
    lbl_1_bss_8E404[0] = 0;
    lbl_1_bss_8E404[1] = 0;
    lbl_1_bss_8E404[2] = 0;
    lbl_1_bss_8E404[3] = 0;
    lbl_1_bss_8E404[4] = 0;
    lbl_1_bss_8E404[5] = 0;
    lbl_1_bss_8E404[6] = 0;
    lbl_1_bss_8E404[7] = 0;
}
/* fzgx:end fn_1_13F948 */

/* fzgx:begin fn_1_13F974 */
extern u32 lbl_801A6410;
extern char lbl_1_data_419E0[18];
extern u32 fn_1_45D0(u32 arg0, int arg1, void* arg2, int arg3);
extern u32 lbl_1_bss_8E404[8];
extern u32 fn_80008BEC(u32 arg0, int arg1, int arg2);

// Allocate a static-display resource for the selected byte-sized slot and initialize it.
u32 fn_1_13F974(u32 arg0) {
    u32 slot = arg0 & 0xFF;

    lbl_1_bss_8E404[slot] =
        fn_1_45D0(lbl_801A6410, 0x440, lbl_1_data_419E0, 0x1528);
    return fn_80008BEC(lbl_1_bss_8E404[slot], 0, 0x440);
}
/* fzgx:end fn_1_13F974 */

/* fzgx:begin fn_1_13F9DC */
extern u32 lbl_1_bss_8E404[8];

// Returns the static-display object selected by the caller's low-byte slot index.
u32 fn_1_13F9DC(u32 slot) {
    return lbl_1_bss_8E404[slot & 0xFF];
}
/* fzgx:end fn_1_13F9DC */

/* fzgx:begin fn_1_13F9F0 */
extern u32 lbl_1_bss_8E404[8];
extern u32 lbl_801A6410;
extern char lbl_1_data_419E0[18];
extern void fn_1_7F3AC(void* object);
extern void fn_1_46B4(u32 config, void* object, char* data, int size);

typedef struct {
    char pad[0x3A4];
    u32 unk_3A4;
} FnObject;

// Releases each active static-display object before clearing its slot.
void fn_1_13F9F0(void) {
    u8 slot;

    for (slot = 0; slot < 8; slot++) {
        if (lbl_1_bss_8E404[slot] != 0) {
            if (((FnObject*)lbl_1_bss_8E404[slot])->unk_3A4 != 0) {
                fn_1_7F3AC((void*)lbl_1_bss_8E404[slot]);
            }
            fn_1_46B4(lbl_801A6410, (void*)lbl_1_bss_8E404[slot],
                lbl_1_data_419E0, 0x153E);
        }
        lbl_1_bss_8E404[slot] = 0;
    }
}
/* fzgx:end fn_1_13F9F0 */

/* fzgx:begin fn_1_1420A4 */
#include "rel/main_rel/sel_static_disp.h"

void fn_1_1420A4(void) {
    ((void (**)(void))lbl_1_data_4244C)[lbl_1_bss_8E51D]();
}
/* fzgx:end fn_1_1420A4 */

/* fzgx:begin fn_1_144E90 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_144F40(u32 arg0, u32 arg1);

void fn_1_144E90(void) {
    lbl_1_bss_8E428.unk_0 = (u32)&lbl_1_bss_3C30.unk_E;
    if (lbl_1_bss_8E51D == lbl_1_bss_8E524) {
        fn_1_144F40(0, 0);
    }
}
/* fzgx:end fn_1_144E90 */

/* fzgx:begin fn_1_144EE8 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_144F40(u32 arg0, u32 arg1);

void fn_1_144EE8(void) {
    lbl_1_bss_8E428.unk_0 = (u32)((u8*)&lbl_1_bss_3C30 + 0x1394);
    if (lbl_1_bss_8E51D == lbl_1_bss_8E524) {
        fn_1_144F40(1, 0);
    }
}
/* fzgx:end fn_1_144EE8 */

/* fzgx:begin fn_1_149B64 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_149CA4(u32 arg0, u32 arg1, u32 arg2, u32 arg3);
extern void fn_1_5233C(void* arg0, void* arg1);
extern u32 fn_1_A5DB0(void);
extern u32 fn_1_A5D88(void);
extern void fn_80074188(u32 arg0, u32 arg1, u32 arg2, u32 arg3);
extern void fn_1_149C2C(void* arg0, void* arg1);

// Refresh the display state before publishing the current display and effect IDs.
void fn_1_149B64(void* display_state, void* display_context) {
    u32 display_id;
    u32 effect_id;

    fn_1_149CA4(lbl_1_bss_8E428.unk_C,
                 *(u32*)((u8*)&lbl_1_bss_8E428 + 0x14),
                 lbl_1_bss_8E428.unk_10,
                 *(u32*)((u8*)&lbl_1_bss_8E428 + 0x18));
    fn_1_5233C(display_state, display_context);

    display_id = fn_1_A5DB0() & 0xffff;
    effect_id = fn_1_A5D88() & 0xffff;
    fn_80074188(0, 0, effect_id, display_id);

    fn_1_149C2C(display_state, display_context);
}
/* fzgx:end fn_1_149B64 */

/* fzgx:begin fn_1_149BF4 */
extern u32 lbl_1_bss_8E428;
extern void fn_1_149CA4(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

// Load four values from BSS table and pass to fn_1_149CA4.
void fn_1_149BF4(void) {
    u8* base = (u8*)&lbl_1_bss_8E428;
    u32 arg0 = *(u32*)(base + 0x4);
    u32 arg1 = *(u32*)(base + 0x14);
    u32 arg2 = *(u32*)(base + 0x8);
    u32 arg3 = *(u32*)(base + 0x18);
    fn_1_149CA4(arg0, arg1, arg2, arg3);
}
/* fzgx:end fn_1_149BF4 */

/* fzgx:begin fn_1_149C2C */
extern u32 lbl_1_bss_8E428;
extern void fn_1_149CA4(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

// Forward the four current static-display values to the display-update routine.
void fn_1_149C2C(void) {
    u8* table = (u8*)&lbl_1_bss_8E428;
    u32 position = *(u32*)(table + 0xC);
    u32 direction = *(u32*)(table + 0x14);
    u32 scale = *(u32*)(table + 0x10);
    u32 flags = *(u32*)(table + 0x18);

    fn_1_149CA4(position, direction, scale, flags);
}
/* fzgx:end fn_1_149C2C */

/* fzgx:begin fn_1_149DA4 */
extern u8 lbl_1_bss_8E51D;
extern u8 lbl_1_bss_8E524;
extern void fn_1_144F40(u32 arg0, u32 arg1);

void fn_1_149DA4(void) {
    if (lbl_1_bss_8E51D == lbl_1_bss_8E524) {
        fn_1_144F40(4, 0);
    }
}
/* fzgx:end fn_1_149DA4 */

/* fzgx:begin fn_1_149DE8 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_144F40(u32 arg0, u32 arg1);

void fn_1_149DE8(void) {
    if (lbl_1_bss_8E51D == lbl_1_bss_8E524) {
        fn_1_144F40(5, 0);
    }
}
/* fzgx:end fn_1_149DE8 */

/* fzgx:begin fn_1_149F04 */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_144F40(int, int);

void fn_1_149F04(void) {
    lbl_1_bss_8E428.unk_0 = (u32)((u8 *)&lbl_1_bss_3C30 + 0x1394);
    if (lbl_1_bss_8E51D == lbl_1_bss_8E524) {
        fn_1_144F40(3, 0);
    }
}
/* fzgx:end fn_1_149F04 */

/* fzgx:begin fn_1_149F5C */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_144F40(int, int);

void fn_1_149F5C(void) {
    lbl_1_bss_8E428.unk_0 = (u32)&lbl_1_bss_3C30 + 0xe;
    if (lbl_1_bss_8E51D != lbl_1_bss_8E524) {
        return;
    }
    fn_1_144F40(7, 0);
}
/* fzgx:end fn_1_149F5C */

/* fzgx:begin fn_1_149FB4 */
#include "rel/main_rel/sel_static_disp.h"

extern u8 fn_1_155F8C(void);

u8 fn_1_149FB4(s32 index) {
    u8 *entry = (u8 *)&lbl_1_bss_26314 + index * 0x1fc;

    if (lbl_1_bss_3C30.unk_5 == 2) {
        return fn_1_155F8C();
    }

    return entry[0x115];
}
/* fzgx:end fn_1_149FB4 */

/* fzgx:begin fn_1_14A000 */
// fn_1_14A000: empty in retail (single blr).
void fn_1_14A000(void) {
}
/* fzgx:end fn_1_14A000 */

/* fzgx:begin fn_1_14A004 */
// fn_1_14A004: empty in retail (single blr).
void fn_1_14A004(void) {
}
/* fzgx:end fn_1_14A004 */

/* fzgx:begin fn_1_14A008 */
// fn_1_14A008: empty in retail (single blr).
void fn_1_14A008(void) {
}
/* fzgx:end fn_1_14A008 */

/* fzgx:begin fn_1_14A00C */
extern u32 lbl_1_bss_8E520;
extern u8 lbl_1_bss_8E524;

void fn_1_14A00C(void) {
    lbl_1_bss_8E520 = 1;
    lbl_1_bss_8E524 = 0xff;
}
/* fzgx:end fn_1_14A00C */

/* fzgx:begin fn_1_14A17C */
typedef struct {
    u8 pad_0[0x18];
    f32 unk_18;
    f32 unk_1c;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u32 unk_2c;
} Obj_1_bss_8E518;

extern const f32 lbl_1_rodata_99A4;
extern Obj_1_bss_8E518 lbl_1_bss_8E518;

// Reset the static display state to its initial values.
void fn_1_14A17C(void) {
    lbl_1_bss_8E518.unk_20 = 0;
    lbl_1_bss_8E518.unk_18 = lbl_1_rodata_99A4;
    lbl_1_bss_8E518.unk_1c = lbl_1_rodata_99A4;
    lbl_1_bss_8E518.unk_24 = 0;
    lbl_1_bss_8E518.unk_28 = 0;
    lbl_1_bss_8E518.unk_2c = 0;
}
/* fzgx:end fn_1_14A17C */

/* fzgx:begin fn_1_14A278 */
// fn_1_14A278: empty in retail (single blr).
void fn_1_14A278(void) {
}
/* fzgx:end fn_1_14A278 */

/* fzgx:begin fn_1_14A27C */
// fn_1_14A27C: empty in retail (single blr).
void fn_1_14A27C(void) {
}
/* fzgx:end fn_1_14A27C */

/* fzgx:begin fn_1_14A280 */
// fn_1_14A280: empty in retail (single blr).
void fn_1_14A280(void) {
}
/* fzgx:end fn_1_14A280 */

/* fzgx:begin fn_1_14A9CC */
// fn_1_14A9CC: empty in retail (single blr).
void fn_1_14A9CC(void) {
}
/* fzgx:end fn_1_14A9CC */

/* fzgx:begin fn_1_14AE94 */
#include "rel/main_rel/sel_static_disp.h"

extern u32 lbl_801A66A4;

void fn_1_14AE94(void) {
    if ((s32)lbl_1_bss_8E540 != 0) {
        lbl_1_bss_8E528 = lbl_801A66A4 - 0xbe;
    }
    if ((s32)lbl_1_bss_8E540 == 0) {
        lbl_1_bss_8E540 = 1;
    }
}
/* fzgx:end fn_1_14AE94 */

/* fzgx:begin fn_1_14B1B8 */
// fn_1_14B1B8: empty in retail (single blr).
void fn_1_14B1B8(void) {
}
/* fzgx:end fn_1_14B1B8 */

/* fzgx:begin fn_1_14B1BC */
#include "rel/main_rel/sel_static_disp.h"

extern u32 lbl_801A66A4;

void fn_1_14B1BC(void) {
    int flag = lbl_1_bss_8E544;

    if (flag != 0) {
        lbl_1_bss_8E528 = lbl_801A66A4 - 0xbe;
    }
    if (flag == 0) {
        lbl_1_bss_8E544 = 1;
    }
}
/* fzgx:end fn_1_14B1BC */

/* fzgx:begin fn_1_14B4FC */
// fn_1_14B4FC: empty in retail (single blr).
void fn_1_14B4FC(void) {
}
/* fzgx:end fn_1_14B4FC */

/* fzgx:begin fn_1_14B8F8 */
// fn_1_14B8F8: empty in retail (single blr).
void fn_1_14B8F8(void) {
}
/* fzgx:end fn_1_14B8F8 */

/* fzgx:begin fn_1_14B8FC */
#include "rel/main_rel/sel_static_disp.h"

extern u32 lbl_801A66A4;

void fn_1_14B8FC(void) {
    s32 flag = lbl_1_bss_8E544;
    if (flag != 0) {
        lbl_1_bss_8E528 = lbl_801A66A4 - 0xbe;
    }
    if (flag == 0) {
        lbl_1_bss_8E544 = 1;
    }
}
/* fzgx:end fn_1_14B8FC */

/* fzgx:begin fn_1_14BC3C */
// fn_1_14BC3C: empty in retail (single blr).
void fn_1_14BC3C(void) {
}
/* fzgx:end fn_1_14BC3C */

/* fzgx:begin fn_1_14BD44 */
extern void fn_1_14BFD8(void *, void *, int);

void fn_1_14BD44(void *arg0, void *arg1) {
    fn_1_14BFD8(arg1, arg0, 0);
}
/* fzgx:end fn_1_14BD44 */

/* fzgx:begin fn_1_14BD74 */
extern void fn_1_14BFB8(void);

void fn_1_14BD74(void) {
    fn_1_14BFB8();
}
/* fzgx:end fn_1_14BD74 */

/* fzgx:begin fn_1_14BFB8 */
extern u8 lbl_1_bss_8E51D;
extern u32 lbl_1_bss_8E520;

void fn_1_14BFB8(void) {
    if (lbl_1_bss_8E51D != 0) {
        lbl_1_bss_8E520 = 1;
    }
}
/* fzgx:end fn_1_14BFB8 */

/* fzgx:begin fn_1_14C7F4 */
// fn_1_14C7F4: empty in retail (single blr).
void fn_1_14C7F4(void) {
}
/* fzgx:end fn_1_14C7F4 */

/* fzgx:begin fn_1_14C7F8 */
#include "rel/main_rel/sel_static_disp.h"

extern s32 fn_1_3F864(void);
extern void fn_1_A2D84(u32 value);
extern u32 lbl_801A66A4;

void fn_1_14C7F8(void) {
    s32 state;

    if (fn_1_3F864() != 0) {
        fn_1_A2D84(0xa9070000);
    }

    state = lbl_1_bss_8E540;
    if (state != 0) {
        lbl_1_bss_8E528 = lbl_801A66A4 - 0xbe;
    }
    if (state == 0) {
        lbl_1_bss_8E540 = 1;
    }
}
/* fzgx:end fn_1_14C7F8 */

/* fzgx:begin fn_1_14C864 */
#include "rel/main_rel/sel_static_disp.h"

extern u32 lbl_801A66A4;

extern void fn_1_14A9D0(u8, int, int, int, int, int);
extern void fn_1_A2D84(u32);
extern void fn_1_14CA4C(void);

void fn_1_14C864(void) {
    Obj_1_bss_9F8 *flags;
    u8 mode;

    if ((s32)lbl_1_bss_25BA0.unk_0 != 0) {
        return;
    }

    if (lbl_1_bss_8E51D == 4) {
        mode = lbl_1_bss_3C30.unk_8;
    } else {
        mode = lbl_1_bss_3C30.unk_9;
    }

    fn_1_14A9D0(mode, 1, 30, 180, 1, 0);

    flags = &lbl_1_bss_9F8;
    if (((flags->unk_8 >> 8) & 1) != 0) {
        if (((lbl_1_bss_D58.unk_8 >> 12) & 1) == 0) {
            if (lbl_801A66A4 - lbl_1_bss_8E528 >= 0xC3) {
                lbl_1_bss_8E51D = 8;
                fn_1_A2D84(0xA9010400);
            } else {
                fn_1_14CA4C();
            }
            return;
        }
    }

    if (((flags->unk_8 >> 9) & 1) == 0) {
        return;
    }

    lbl_1_bss_8E51D = 0;
    fn_1_A2D84(0xA9010400);
}
/* fzgx:end fn_1_14C864 */

/* fzgx:begin fn_1_14C964 */
// fn_1_14C964: empty in retail (single blr).
void fn_1_14C964(void) {
}
/* fzgx:end fn_1_14C964 */

/* fzgx:begin fn_1_14C968 */
#include "rel/main_rel/sel_static_disp.h"

void fn_1_14C968(void) {
    if (lbl_1_bss_3C30.unk_5 == 2) {
        fn_1_3FF90();
    }
    fn_1_C2454();
}
/* fzgx:end fn_1_14C968 */

/* fzgx:begin fn_1_14CA3C */
// fn_1_14CA3C: empty in retail (single blr).
void fn_1_14CA3C(void) {
}
/* fzgx:end fn_1_14CA3C */

/* fzgx:begin fn_1_14CA40 */
// fn_1_14CA40: empty in retail (single blr).
void fn_1_14CA40(void) {
}
/* fzgx:end fn_1_14CA40 */

/* fzgx:begin fn_1_14CA44 */
// fn_1_14CA44: empty in retail (single blr).
void fn_1_14CA44(void) {
}
/* fzgx:end fn_1_14CA44 */

/* fzgx:begin fn_1_14CA48 */
// fn_1_14CA48: empty in retail (single blr).
void fn_1_14CA48(void) {
}
/* fzgx:end fn_1_14CA48 */

/* fzgx:begin fn_1_14CA4C */
#include "rel/main_rel/sel_static_disp.h"

extern void fn_1_A2D84(u32, u8 *);

// Arms the display update and refreshes it when the selected value exceeds the current one.
void fn_1_14CA4C(void) {
    u8 *display_state = (u8 *)&lbl_1_bss_8E518;
    u8 selected_limit;

    if (lbl_1_bss_8E518.unk_4 != 0) {
        return;
    }

    lbl_1_bss_8E518.unk_4 = 1;
    selected_limit = display_state[5] == 4
        ? lbl_1_bss_3C30.unk_8
        : lbl_1_bss_3C30.unk_9;

    if (selected_limit > display_state[0x30]) {
        fn_1_A2D84(0xA9010400, display_state);
    }
}
/* fzgx:end fn_1_14CA4C */

/* fzgx:begin fn_1_14D5CC */
extern u32 lbl_801A63C0;
extern s16 fn_1_14F01C(void *arg0);
extern s16 fn_1_14F090(void *arg0, s16 arg1);
extern u32 fn_1_14D6D8(s16 index);

// Generate candidates until one matches the requested flags without forbidden flags.
s16 fn_1_14D5CC(void *arg0, u32 required_mask, u32 forbidden_mask) {
    s16 candidate_index;
    s32 value;

    do {
        lbl_801A63C0 = lbl_801A63C0 * 0x676a4b6b + 0x33cb;
        value = (lbl_801A63C0 >> 16) & 0x7fff;
        candidate_index =
            fn_1_14F090(arg0, (s16)(value % fn_1_14F01C(arg0)));
        value = fn_1_14D6D8(candidate_index);
    } while ((required_mask & value) == 0 ||
             (forbidden_mask & fn_1_14D6D8(candidate_index)) != 0);

    return candidate_index;
}
/* fzgx:end fn_1_14D5CC */

/* fzgx:begin fn_1_14D670 */
extern u32 lbl_801A63C0;

s16 fn_1_14D670(void) {
    u32 value;
    s32 sample;
    s16 result;

    do {
        value = lbl_801A63C0 * 0x676a4b6b + 0x33cb;
        lbl_801A63C0 = value;
        sample = (value >> 16) & 0x7fff;
        result = sample % 30;
    } while (result == 2 || (u16)(result - 4) <= 2);

    return result;
}
/* fzgx:end fn_1_14D670 */

/* fzgx:begin fn_1_14D6D8 */
extern u32 lbl_1_rodata_99D8[75];

typedef struct {
    u32 values[75];
} Table75;

u32 fn_1_14D6D8(s16 index) {
    Table75 values = *(Table75 *)lbl_1_rodata_99D8;

    return values.values[index];
}
/* fzgx:end fn_1_14D6D8 */

/* fzgx:begin fn_1_14DBCC */
typedef struct {
    u8 pad[0x340];
    void *field_340;
    void *fields_344[4];
    u8 tail[0xc];
} Fn1_14DBCC_Item;

extern void fn_800711A8(void *arg0);
extern void fn_80071718(void *arg0);

void fn_1_14DBCC(Fn1_14DBCC_Item *arg0) {
    Fn1_14DBCC_Item *base;
    s16 i;
    s16 j;
    Fn1_14DBCC_Item *item;

    base = arg0;
    for (i = 0; (u32)i < 3; i++) {
        item = base;
        for (j = 0; j < 4; j++) {
            if (item->fields_344[j] != 0) {
                fn_800711A8(item->fields_344[j]);
                item->fields_344[j] = 0;
            }
        }
        if (base->field_340 != 0) {
            fn_80071718(base->field_340);
            base->field_340 = 0;
        }
        base++;
    }
}
/* fzgx:end fn_1_14DBCC */

/* fzgx:begin fn_1_14DE80 */
extern void fn_1_14DEC8(void *arg0, void *arg1, void *arg2, void *arg3, int arg4);

void fn_1_14DE80(void *arg0, void *arg1, void *arg2, void *arg3) {
    fn_1_14DEC8(arg0, arg1, arg2, arg3, 0);
}
/* fzgx:end fn_1_14DE80 */

/* fzgx:begin fn_1_14DEA4 */
extern void fn_1_14DEC8(void *arg0, void *arg1, void *arg2, void *arg3, int arg4);

void fn_1_14DEA4(void *arg0, void *arg1, void *arg2, void *arg3) {
    fn_1_14DEC8(arg0, arg1, arg2, arg3, 1);
}
/* fzgx:end fn_1_14DEA4 */

/* fzgx:begin fn_1_14E054 */
extern void fn_1_14E09C(void *arg0, void *arg1, void *arg2, int arg3);

void fn_1_14E054(void *arg0, void *arg1, void *arg2) {
    fn_1_14E09C(arg0, arg1, arg2, 0);
}
/* fzgx:end fn_1_14E054 */

/* fzgx:begin fn_1_14E078 */
extern void fn_1_14E09C(void *arg0, void *arg1, void *arg2, int arg3);

void fn_1_14E078(void *arg0, void *arg1, void *arg2) {
    fn_1_14E09C(arg0, arg1, arg2, 1);
}
/* fzgx:end fn_1_14E078 */

/* fzgx:begin fn_1_14E198 */
extern void fn_1_14E1E0(void *arg0, void *arg1, void *arg2, int arg3);

void fn_1_14E198(void *arg0, void *arg1, void *arg2) {
    fn_1_14E1E0(arg0, arg1, arg2, 0);
}
/* fzgx:end fn_1_14E198 */

/* fzgx:begin fn_1_14E1BC */
extern void fn_1_14E1E0(void *arg0, void *arg1, void *arg2, int arg3);

void fn_1_14E1BC(void *arg0, void *arg1, void *arg2) {
    fn_1_14E1E0(arg0, arg1, arg2, 1);
}
/* fzgx:end fn_1_14E1BC */

/* fzgx:begin fn_1_14E8A4 */
typedef struct {
    s16 value;
    s16 sort_key;
} Fn1_14E8A4Entry;

int fn_1_14E8A4(const Fn1_14E8A4Entry *arg0, const Fn1_14E8A4Entry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg0->sort_key - arg1->sort_key;
}
/* fzgx:end fn_1_14E8A4 */

/* fzgx:begin fn_1_14E8CC */
typedef struct {
    s16 value;
    u8 pad[8];
    s16 sort_key;
} Fn1_14E8CCEntry;

int fn_1_14E8CC(const Fn1_14E8CCEntry *arg0, const Fn1_14E8CCEntry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg0->sort_key - arg1->sort_key;
}
/* fzgx:end fn_1_14E8CC */

/* fzgx:begin fn_1_14E8F4 */
typedef struct {
    s16 value;
    u8 padding[8];
    s16 sort_key;
} Fn1_14E8F4Entry;

int fn_1_14E8F4(const Fn1_14E8F4Entry *arg0, const Fn1_14E8F4Entry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg1->sort_key - arg0->sort_key;
}
/* fzgx:end fn_1_14E8F4 */

/* fzgx:begin fn_1_14E91C */
typedef struct {
    s16 value;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 sort_key;
} Fn1_14E91CEntry;

int fn_1_14E91C(const Fn1_14E91CEntry *arg0, const Fn1_14E91CEntry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg0->sort_key - arg1->sort_key;
}
/* fzgx:end fn_1_14E91C */

/* fzgx:begin fn_1_14E944 */
typedef struct {
    s16 value;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 sort_key;
} Fn1_14E944Entry;

int fn_1_14E944(const Fn1_14E944Entry *arg0, const Fn1_14E944Entry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg1->sort_key - arg0->sort_key;
}
/* fzgx:end fn_1_14E944 */

/* fzgx:begin fn_1_14EEC4 */
extern u8 lbl_1_rodata_B884[76];
extern void fn_80083DB0(void *obj, u32 value);

typedef struct {
    u32 values[3][6];
} StaticDispTable;

void *fn_1_14EEC4(s16 index, void *obj, s16 subindex) {
    StaticDispTable table;

    table = *(StaticDispTable *)lbl_1_rodata_B884;
    fn_80083DB0(obj, table.values[index][subindex]);
    return obj;
}
/* fzgx:end fn_1_14EEC4 */

/* fzgx:begin fn_1_14F014 */
// fn_1_14F014: returns a constant.
int fn_1_14F014(void) {
    return 75;
}
/* fzgx:end fn_1_14F014 */

/* fzgx:begin fn_1_14F01C */
extern u8 lbl_1_rodata_B8D8[1200];

typedef struct {
    u8 unk0[2];
    s16 value;
    u8 unk4[12];
} Entry;

typedef struct {
    Entry entries[75];
} Table;

// Count entries in the static display table whose value matches the argument.
s16 fn_1_14F01C(s16 value) {
    Table table;
    s16 count;
    s16 i;

    table = *(Table *)lbl_1_rodata_B8D8;
    i = 0;
    count = 0;
    while (i < 75) {
        if (table.entries[i].value == value) {
            count++;
        }
        i++;
    }
    return count;
}
/* fzgx:end fn_1_14F01C */

/* fzgx:begin fn_1_14F090 */
extern u8 lbl_1_rodata_BD88[2400];

typedef struct {
    u8 unk0[2];
    s16 value;
    u8 unk4[12];
} Entry;

typedef struct {
    Entry entries[75];
} Table;

// Finds the index of the requested occurrence of a value in the static table.
s16 fn_1_14F090(s16 value, s16 occurrence) {
    Table table;
    s16 i;
    s16 count;

    table = *(Table *)lbl_1_rodata_BD88;
    i = 0;
    count = 0;
    while (i < 75) {
        if (table.entries[i].value == value) {
            if (count == occurrence) {
                return i;
            }
            count++;
        }
        i++;
    }
    return -1;
}
/* fzgx:end fn_1_14F090 */

/* fzgx:begin fn_1_14F118 */
extern s16 fn_1_14F090(void *table, s16 index);
extern s16 fn_1_14F01C(void *table);

// Returns the index of the first table entry matching the requested value.
s16 fn_1_14F118(s16 value, void *table) {
    s16 index;

    for (index = 0; index < fn_1_14F01C(table); index++) {
        if (fn_1_14F090(table, index) == value) {
            return index;
        }
    }
    return -1;
}
/* fzgx:end fn_1_14F118 */

/* fzgx:begin fn_1_14F19C */
extern f32 lbl_1_rodata_99D8[75];
extern s16 fn_1_14F090(void *arg, s16 index);
extern s16 fn_1_14F01C(void *arg);

typedef struct {
    u32 values[75];
} FlagTable;

// Returns the ordinal among enabled entries for the requested value, or -1 if absent.
s16 fn_1_14F19C(s16 wanted, void *arg, u32 mask) {
    FlagTable table;
    s16 entry_index;
    s16 enabled_ordinal;

    entry_index = 0;
    enabled_ordinal = 0;
    for (; entry_index < fn_1_14F01C(arg); entry_index++) {
        s16 value;

        value = fn_1_14F090(arg, entry_index);
        table = *(FlagTable *)lbl_1_rodata_99D8;
        if (mask & table.values[value]) {
            if (value == wanted) {
                return enabled_ordinal;
            }
            enabled_ordinal++;
        }
    }
    return -1;
}
/* fzgx:end fn_1_14F19C */

/* fzgx:begin fn_1_14F344 */
extern u8 lbl_1_rodata_C6E8[1200];

typedef struct {
    u8 unk0[2];
    s16 value;
    u8 unk4[12];
} Entry;

typedef struct {
    Entry entries[75];
} Table;

s16 fn_1_14F344(s16 index) {
    Table table;

    table = *(Table *)lbl_1_rodata_C6E8;
    return table.entries[index].value;
}
/* fzgx:end fn_1_14F344 */

/* fzgx:begin fn_1_14FC80 */
extern void *fn_1_36AD0(s16 arg0, u32 arg1);
extern void fn_1_14F6F8(u8 arg0, u8 arg1, u8 arg2, u32 arg3);

void fn_1_14FC80(s16 arg0, u32 arg1) {
    u8 *data;

    data = (u8 *)fn_1_36AD0(arg0, arg1) + (s32)arg0 * 0x81C0;
    fn_1_14F6F8(data[0x81A4], data[0x81AC], data[0x81B4], arg1);
}
/* fzgx:end fn_1_14FC80 */

/* fzgx:begin fn_1_14FCE4 */
extern u8 lbl_1_rodata_CCDC[308];
extern void fn_1_14FD7C(u32 *data, u32 count);
extern void fn_80008BA8(u32 arg0, u32 *data, s32 offset);

typedef struct {
    u32 words[77];
} Data;

void fn_1_14FCE4(u32 arg0, s16 arg1) {
    Data data;

    data = *(Data *)lbl_1_rodata_CCDC;
    fn_1_14FD7C(data.words, 77);
    fn_80008BA8(arg0, data.words, (s32)arg1 << 2);
}
/* fzgx:end fn_1_14FCE4 */

/* fzgx:begin fn_1_14FD6C */
s32 fn_1_14FD6C(s32 *arg0, s32 *arg1) {
    return *arg0 - *arg1;
}
/* fzgx:end fn_1_14FD6C */

/* fzgx:begin fn_1_150500 */
void fn_1_150500(void) {
    lbl_1_data_2A7E0.unk_3C->unk_0 = 0;
}
/* fzgx:end fn_1_150500 */

/* fzgx:begin fn_1_150518 */
extern void fn_1_9A508(Obj_1_data_2A7E0 *state);
extern void fn_1_150C8C(Obj_1_data_2A7E0_At3C *obj);
extern void fn_1_150F30(Obj_1_data_2A7E0_At3C *obj);

// Reset the active object's counters and re-run its two setup passes.
void fn_1_150518(void) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    obj->unk_84 = 0;
    obj->unk_2724 = 0;
    fn_1_9A508(&lbl_1_data_2A7E0);
    fn_1_150C8C(obj);
    fn_1_150F30(obj);
    obj->unk_0 = 1;
}
/* fzgx:end fn_1_150518 */

/* fzgx:begin fn_1_150570 */
// fn_1_150570: empty in retail (single blr).
void fn_1_150570(void) {
}
/* fzgx:end fn_1_150570 */

/* fzgx:begin fn_1_150574 */
// Initializes the static display object referenced by the main state.
void fn_1_150574(void) {
    Obj_1_data_2A7E0_At3C *display = lbl_1_data_2A7E0.unk_3C;

    fn_1_150CEC(display);
    fn_1_151668(display);
}
/* fzgx:end fn_1_150574 */

/* fzgx:begin fn_1_1505B4 */
extern void fn_1_9AD54(void);
extern u32 fn_1_9D260(void);
extern void fn_1_150D3C(Obj_1_data_2A7E0_At3C *arg0, u32 arg1);
extern void fn_1_150654(void);
extern void fn_1_151054(Obj_1_data_2A7E0_At3C *arg);

// Refreshes the static display state and applies the current object selection.
void fn_1_1505B4(void) {
    Obj_1_data_2A7E0_At3C *display_state;
    u32 result;

    display_state = lbl_1_data_2A7E0.unk_3C;
    fn_1_9AD54();
    result = fn_1_9D260();
    fn_1_150D3C(display_state, result);
    fn_1_150654();
    fn_1_151054(display_state);
}
/* fzgx:end fn_1_1505B4 */

/* fzgx:begin fn_1_150608 */
// Prepare the static display state and pass it to the display routine.
void fn_1_150608(void) {
    Obj_1_data_2A7E0_At3C *value;
    u32 result;

    value = lbl_1_data_2A7E0.unk_3C;
    fn_1_9AD88();
    result = fn_1_9D260();
    fn_1_150ED0(value, result);
}
/* fzgx:end fn_1_150608 */

/* fzgx:begin fn_1_150650 */
// fn_1_150650: empty in retail (single blr).
void fn_1_150650(void) {
}
/* fzgx:end fn_1_150650 */

/* fzgx:begin fn_1_150C8C */
extern void fn_1_103090(void *entry);

typedef struct Entry {
    u8 pad[0x68];
    u32 active;
    u8 tail[0x40];
} Entry;

typedef struct Object {
    u8 pad_84[0x84];
    s32 count;
    Entry entries[1];
} Object;

// Marks each static display entry active before updating it.
void fn_1_150C8C(Object *obj) {
    s32 count;
    Entry *entry;

    count = obj->count;
    entry = obj->entries;
    while (count > 0) {
        entry->active = 1;
        fn_1_103090(entry);
        count -= 1;
        entry += 1;
    }
}
/* fzgx:end fn_1_150C8C */

/* fzgx:begin fn_1_150CEC */
extern void fn_1_1030A4(void *entry);

typedef struct Entry {
    u8 data[0xac];
} Entry;

typedef struct Object {
    u8 pad_84[0x84];
    s32 count;
    Entry entries[1];
} Object;

void fn_1_150CEC(Object *obj) {
    s32 count;
    Entry *entry;

    count = obj->count;
    entry = obj->entries;
    while (count > 0) {
        fn_1_1030A4(entry);
        count -= 1;
        entry += 1;
    }
}
/* fzgx:end fn_1_150CEC */

/* fzgx:begin fn_1_150ED0 */
extern void fn_1_103264(void *entry, void *arg);

typedef struct Entry {
    u8 data[0xac];
} Entry;

typedef struct Object {
    u8 pad_84[0x84];
    s32 count;
    Entry entries[1];
} Object;

void fn_1_150ED0(Object *obj, void *arg) {
    s32 count;
    Entry *entry;

    count = obj->count;
    entry = obj->entries;
    while (count > 0) {
        fn_1_103264(entry, arg);
        count -= 1;
        entry += 1;
    }
}
/* fzgx:end fn_1_150ED0 */

/* fzgx:begin fn_1_150F30 */
extern void fn_1_150F74(void *self);
extern void fn_1_151668(void *self);

typedef struct StaticDisp {
    u8 pad_2728[0x2728];
    s32 unk_2728;
    s32 unk_272c;
} StaticDisp;

// Reset the display state before rebuilding its static entries.
void fn_1_150F30(StaticDisp *self) {
    self->unk_2728 = 0;
    self->unk_272c = -1;
    fn_1_150F74(self);
    fn_1_151668(self);
}
/* fzgx:end fn_1_150F30 */

/* fzgx:begin fn_1_150F74 */
extern const f32 lbl_1_rodata_CF40[];

typedef struct Object {
    u8 pad_0[0x2730];
    f32 value_2730;
    f32 value_2734;
    f32 value_2738;
    f32 value_273c;
    u8 pad_2740[4];
    f32 value_2744;
} Object;

typedef struct Entry {
    u8 pad_0[0x1824];
    f32 value_1824;
    f32 value_1828;
    f32 value_182c;
    f32 value_1830;
    f32 value_1834;
    f32 value_1838;
    f32 value_183c;
    f32 value_1840;
    f32 value_1844;
    f32 value_1848;
    u8 pad_184c[0xc];
    u32 *bits;
    u8 flag_185c;
    u8 flag_185d;
    u8 flag_185e;
    u8 flag_185f;
} Entry;

// Initialize display values and propagate enabled display flags across all entries.
void fn_1_150F74(Object *obj) {
    const f32 *constants;
    Entry *entry;
    u32 *bit_flags;
    s32 index;

    constants = lbl_1_rodata_CF40;
    obj->value_2730 = constants[58];
    obj->value_2734 = constants[23];
    obj->value_2738 = constants[23];
    obj->value_273c = constants[59];
    obj->value_2744 = constants[60];

    entry = (Entry *)obj;
    for (index = 0; index < 0x40; index++) {
        bit_flags = entry->bits;
        if (entry->flag_185c != 0) {
            entry->value_1824 = obj->value_2730;
            entry->value_1828 = constants[23];
            entry->value_182c = constants[23];
            *bit_flags |= ((u32)1 << 31);
        }
        if (entry->flag_185d != 0) {
            entry->value_1824 = constants[23];
            entry->value_1828 = constants[23];
            entry->value_182c = constants[23];
            entry->value_1830 = constants[23];
        }
        if (entry->flag_185d != 0 || entry->flag_185e != 0) {
            entry->value_1834 = constants[23];
            entry->value_1840 = constants[23];
            entry->value_1838 = constants[23];
            entry->value_1844 = constants[23];
            entry->value_183c = constants[23];
            entry->value_1848 = constants[23];
        }
        if (entry->flag_185f != 0) {
            *bit_flags &= 0x7fffffff;
        }
        entry = (Entry *)((u8 *)entry + 0x3c);
    }
}
/* fzgx:end fn_1_150F74 */

/* fzgx:begin fn_1_151764 */
// fn_1_151764: returns a constant.
int fn_1_151764(void) {
    return 1;
}
/* fzgx:end fn_1_151764 */
