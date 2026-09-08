#include "types.h"

typedef struct PrepBuffer {
    u8 pad[0x18];
    u32 field_18;
    u32 field_1c;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2c;
    u32 field_30;
    u32 field_34;
} PrepBuffer;

void __prep_buffer(PrepBuffer* buffer) {
    buffer->field_24 = buffer->field_1c;
    buffer->field_28 = buffer->field_20;
    buffer->field_28 -= buffer->field_18 & buffer->field_2c;
    buffer->field_34 = buffer->field_18;
}
