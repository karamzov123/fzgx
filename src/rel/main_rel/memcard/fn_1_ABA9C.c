#include "types.h"

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 pad[0x24];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_ABA9C(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 0x12c;
}
