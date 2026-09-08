#include "types.h"

/* fzgx:begin fn_1_4811C */
extern void fn_1_48214(void *arg, int value);

void fn_1_4811C(void *arg) {
    fn_1_48214(arg, 1);
}
/* fzgx:end fn_1_4811C */

/* fzgx:begin fn_1_48140 */
extern void fn_1_48214(void *, int);

void fn_1_48140(void *arg) {
    fn_1_48214(arg, 0);
}
/* fzgx:end fn_1_48140 */

/* fzgx:begin fn_1_48164 */
#include "rel/main_rel/bitmap.h"

extern void fn_1_48214(int, int);

typedef struct {
    s32 unk_0;
    u8 pad_4[0x20];
    u8 unk_24;
} BitmapEntry;

void fn_1_48164(int value) {
    s16 i;
    BitmapEntry *entry;

    i = 1;
    entry = (BitmapEntry *)((u8 *)&lbl_1_data_FCD4 + 0x28);
    for (; i < 0xbc; i++, entry++) {
        if (entry->unk_0 != 0 && entry->unk_24 == value) {
            fn_1_48214(i, 1);
        }
    }
}
/* fzgx:end fn_1_48164 */

/* fzgx:begin fn_1_481E8 */
extern void fn_1_48214(void *arg0, int arg1);
extern void fn_1_4DDC0(void);
extern void fn_1_4F724(void);

void fn_1_481E8(void *arg0) {
    fn_1_48214(arg0, 0);
    fn_1_4DDC0();
    fn_1_4F724();
}
/* fzgx:end fn_1_481E8 */

/* fzgx:begin fn_1_48418 */
#include "rel/main_rel/bitmap.h"

typedef struct {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
} BitmapSlot;

extern s32 lbl_801A6410;
extern void fn_8000C49C(const unsigned char *, ...);
extern int fn_1_45D0(int, int, const unsigned char *, int);
extern int fn_80008F88(int, int);

void fn_1_48418(int index) {
    BitmapSlot *obj = (BitmapSlot *)&lbl_1_data_6CA0 + index;
    int valid;
    if (index > 2) {
        valid = 0;
    } else {
        valid = index >= 0;
    }
    if (valid == 0) {
        fn_8000C49C(lbl_1_data_1A368, 0x248, lbl_1_data_1A390);
    }
    if (obj->unk_0 == -1) {
        obj->unk_4 = fn_1_45D0(lbl_801A6410, obj->unk_8, lbl_1_data_1A368, 0x250);
        obj->unk_0 = fn_80008F88(obj->unk_4, obj->unk_4 + obj->unk_8);
    }
}
/* fzgx:end fn_1_48418 */

/* fzgx:begin fn_1_484CC */
#include "rel/main_rel/bitmap.h"

extern u32 lbl_801A6410;
extern void fn_8006FDEC(void);
extern void fn_80009064(u32 value);
extern void fn_1_48214(s32 index, s32 value);
extern void fn_1_46B4(u32 arg0, u32 arg1, char *arg2, s32 arg3);

void fn_1_484CC(s32 index) {
    u8 *record;
    s16 i;
    u32 *entry;

    entry = (u32 *)((u8 *)&lbl_1_data_6CA0 + index * 12);
    if (*(s32 *)entry == -1) {
        return;
    }

    fn_8006FDEC();
    i = 1;
    record = (u8 *)&lbl_1_data_FCD4 + 0x28;
    while (i < 188) {
        if (*(s32 *)record != 0 && record[0x24] == index) {
            fn_1_48214(i, 1);
        }
        i++;
        record += 0x28;
    }

    fn_80009064(entry[0]);
    fn_1_46B4(lbl_801A6410, entry[1],
              (char *)lbl_1_data_1A368, 0x265);
    entry[0] = (u32)-1;
}
/* fzgx:end fn_1_484CC */

/* fzgx:begin fn_1_485A8 */
#include "rel/main_rel/bitmap.h"

u32 fn_1_485A8(s32 index) {
    return ((Obj_1_data_FCD4 *)((u8 *)&lbl_1_data_FCD4 + index * 0x28))->unk_0 != 0;
}
/* fzgx:end fn_1_485A8 */

/* fzgx:begin fn_1_48690 */
extern void *fn_1_48730(void);

u16 fn_1_48690(void) {
    void *value = fn_1_48730();
    if (value != 0) {
        return *(u16 *)((u8 *)value + 8);
    }
    return 8;
}
/* fzgx:end fn_1_48690 */

/* fzgx:begin fn_1_486C4 */
extern void *fn_1_48730(void);

u16 fn_1_486C4(void) {
    void *value = fn_1_48730();
    if (value != 0) {
        return *(u16 *)((u8 *)value + 0xA);
    }
    return 8;
}
/* fzgx:end fn_1_486C4 */

/* fzgx:begin fn_1_486F8 */
extern void *fn_1_48730(void);

u32 fn_1_486F8(void) {
    void *value = fn_1_48730();
    if (value != 0) {
        return *(u32 *)value & 0x1F;
    }
    return 0;
}
/* fzgx:end fn_1_486F8 */

/* fzgx:begin fn_1_48730 */
#include "rel/main_rel/bitmap.h"

void *fn_1_48730(u32 value) {
    u32 index = (value >> 8) & 0xffff;
    u32 offset = value & 0xff;
    Obj_1_data_FCD4 *entry =
        (Obj_1_data_FCD4 *)((u8 *)&lbl_1_data_FCD4 + index * 0x28);

    if (entry != 0 && (s32)entry->unk_0 == 1 && entry->unk_20 != 0) {
        return (u8 *)entry->unk_20->unk_4 + ((u8)offset << 4);
    }
    return 0;
}
/* fzgx:end fn_1_48730 */

/* fzgx:begin fn_1_4877C */
// fn_1_4877C: empty in retail (single blr).
void fn_1_4877C(void) {
}
/* fzgx:end fn_1_4877C */

/* fzgx:begin fn_1_48780 */
#include "rel/main_rel/bitmap.h"

extern void fn_1_47AD4(Obj_1_data_FCD4 *obj, u8 value, int arg2, int arg3);

void fn_1_48780(void) {
    Obj_1_data_FCD4 *obj;
    int i;

    obj = (Obj_1_data_FCD4 *)((u8 *)&lbl_1_data_FCD4 + 0x28);
    for (i = 1; i < 0xBC; i++) {
        if ((int)obj->unk_0 != 0) {
            fn_1_47AD4(obj, obj->unk_24, 0, 0);
        }
        obj = (Obj_1_data_FCD4 *)((u8 *)obj + 0x28);
    }
}
/* fzgx:end fn_1_48780 */

/* fzgx:begin fn_1_4AEE8 */
#include "rel/main_rel/bitmap.h"

extern f32 lbl_1_rodata_10C0[5];
extern f32 lbl_1_bss_3E05C;

u32 fn_1_4AEE8(u32 value) {
    u32 previous = lbl_1_data_1BCC4;

    lbl_1_bss_3E05C = lbl_1_rodata_10C0[0];
    lbl_1_data_1BCC4 = value;

    return previous;
}
/* fzgx:end fn_1_4AEE8 */
