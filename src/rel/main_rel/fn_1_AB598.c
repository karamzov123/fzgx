#include "types.h"

extern void fn_8002D634(u8 byte_val, int value);

typedef struct {
    u8 pad[0x2];
    u16 field_0x2;
} OffsetStruct;

typedef struct {
    u8 byte_0;
    u8 pad[0x23];
    OffsetStruct *ptr_0x24;
} ArgStruct;

void fn_1_AB598(ArgStruct *arg) {
    arg->ptr_0x24->field_0x2 = 300;
    fn_8002D634(arg->byte_0, 0);
}
