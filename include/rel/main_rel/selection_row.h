#ifndef REL_MAIN_REL_SELECTION_ROW_H
#define REL_MAIN_REL_SELECTION_ROW_H
#include "types.h"

/* Partial physical view, not a claim about field semantics.
 * Retail 0x440 indexing and widths are independently witnessed by
 * fn_1_7D6B8, fn_1_12C110 and fn_1_38CF4. Unknown bytes are never initialized
 * or used to force compiler layout; preserve their storage in this view.
 */
typedef struct FZGXSelectionRow {
    u8 unknown_000[0x324];
    s32 field_324;
    u8 unknown_328[0xC];
    u32 field_334;
    u8 unknown_338[0x54];
    u8 field_38C;
    u8 unknown_38D[3];
    u32 field_390;
    u8 unknown_394[0x24];
    s16 field_3B8;
    u8 unknown_3BA[0x86];
} FZGXSelectionRow;

typedef char FZGXSelectionRow_stride_check[(sizeof(FZGXSelectionRow) == 0x440) ? 1 : -1];
#endif
