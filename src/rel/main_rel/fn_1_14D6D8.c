#include "types.h"

extern u32 lbl_1_rodata_99D8[75];

typedef struct {
    u32 values[75];
} Table75;

u32 fn_1_14D6D8(s16 index) {
    Table75 values = *(Table75 *)lbl_1_rodata_99D8;

    return values.values[index];
}
