#include "types.h"

typedef struct MovieModule {
    u32 unk_00;
    u8 pad_04[8];
    u32 unk_0c;
    u8 pad_10[0x80];
    u32 unk_90;
} MovieModule;

void fn_12_248D4(MovieModule *self) {
    self->unk_00 = 0;
    self->unk_0c = 0;
    self->unk_90 = 0;
}
