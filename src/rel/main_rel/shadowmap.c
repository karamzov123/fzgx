#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/shadowmap.h"

extern void fn_800746A8(u32, u32);
extern void fn_8007245C(u32);
extern void fn_800728A8(u32, u32, u32, u32);
extern void fn_800720B0(u32);
extern void fn_800747D0(u32, u32, u32, u32, u32, u32, u32);
extern void fn_800734A8(u32, u32, u32, u32);
extern void fn_80072EDC(u32, u32);
extern void fn_80073C6C(u32);
extern void fn_80073678(u32);
extern void fn_80074660(u32);
extern void fn_80073898(u32);
extern void fn_80074788(u32);

/* fzgx:begin fn_1_56858 noprologue */
#include "types.h"
#include "rel/main_rel/globals.h"

extern char lbl_1_data_1C660[12];
extern void *lbl_801A6410;
extern void fn_1_46B4(void *, void *, char *, int);

typedef struct ShadowMapEntry {
    char pad_00[0x3c];
    void *unk_3c;
    void *unk_40;
    char pad_44[0x08];
} ShadowMapEntry;

// Register each entry's resources, then clear it for reuse.
void fn_1_56858(ShadowMapEntry *entries, u32 count) {
    u32 i;
    ShadowMapEntry *entry;

    for (i = 0, entry = entries; i < count; i++, entry++) {
        fn_1_46B4(lbl_801A6410, entry->unk_3c, lbl_1_data_1C660, 0x87);
        fn_1_46B4(lbl_801A6410, entry->unk_40, lbl_1_data_1C660, 0x88);
        memset(entry, 0, 0x4c);
    }
}
/* fzgx:end fn_1_56858 */

/* fzgx:begin fn_1_568EC */
typedef struct ShadowMap {
    u8 pad44[0x44];
    u32 field44;
    u32 field48;
} ShadowMap;

void fn_1_568EC(ShadowMap *map, u32 value0, u32 value1) {
    map->field48 = value0;
    map->field44 = value1;
}
/* fzgx:end fn_1_568EC */

/* fzgx:begin fn_1_57714 */
// Updates the shadow-map enable flag.
void fn_1_57714(u8 value) {
    lbl_1_data_1C670.unk_0 = value;
}
/* fzgx:end fn_1_57714 */

/* fzgx:begin fn_1_57720 */
void fn_1_57720(u32 value_1, u32 value_2, u32 value_3, u32 value_4) {
    lbl_1_data_1C670.unk_4 = value_1;
    lbl_1_data_1C670.unk_8 = value_2;
    lbl_1_data_1C670.unk_C = value_3;
    lbl_1_data_1C670.unk_10 = value_4;
}
/* fzgx:end fn_1_57720 */

/* fzgx:begin fn_1_57DC0 */
void fn_1_57DC0(void) {
    fn_800746A8(lbl_1_data_1C670.unk_0, lbl_1_data_1C670.unk_14);
    fn_8007245C(0xa00);
    fn_800728A8(lbl_1_data_1C670.unk_4, lbl_1_data_1C670.unk_8,
                lbl_1_data_1C670.unk_C, lbl_1_data_1C670.unk_10);
    fn_800720B0(0);
    fn_800747D0(4, 0, 1, 1, 0, 2, 1);
    fn_800734A8(0, 0xff, 0xff, 4);
    fn_80072EDC(0, 4);
    fn_80073C6C(0);
    fn_80073678(1);
    fn_80074660(0);
    fn_80073898(0);
    fn_80074788(1);
}
/* fzgx:end fn_1_57DC0 */
