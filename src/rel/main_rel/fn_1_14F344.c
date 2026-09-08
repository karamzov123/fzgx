#include "types.h"

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
