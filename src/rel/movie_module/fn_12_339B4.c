#include "types.h"

extern void fn_12_2A674(u32, u32, u32, void *);

typedef struct MovieResult {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    s16 field_0C;
    s16 field_0E;
} MovieResult;

typedef struct MovieOutput {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
} MovieOutput;

void fn_12_339B4(u32 arg0, u32 arg1, u32 arg2, MovieOutput *out) {
    MovieResult result;
    fn_12_2A674(arg0, arg1, arg2, &result);
    out->field_00 = result.field_08;
    out->field_04 = result.field_00;
    out->field_08 = result.field_04;
    out->field_0C = result.field_0E;
    out->field_10 = result.field_0C;
    out->field_14 = result.field_0C;
}
