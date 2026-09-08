#include "types.h"

typedef struct GXContext {
    u8 pad_0[0x43c];
    f32 field_43c;
    f32 field_440;
    f32 field_444;
    f32 field_448;
    f32 field_44c;
    f32 field_450;
} GXContext;

extern GXContext* gx;

void fn_80038F10(f32* out) {
    GXContext* context = gx;
    out[0] = context->field_43c;
    out[1] = context->field_440;
    out[2] = context->field_444;
    out[3] = context->field_448;
    out[4] = context->field_44c;
    out[5] = context->field_450;
}
