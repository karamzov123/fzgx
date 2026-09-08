#include "types.h"

typedef struct MovieModule {
    u8 pad_00[0x78];
    s32 unk_78;
    u32 unk_7c;
    u8 *unk_80;
} MovieModule;

u8 *fn_12_23F34(MovieModule *self) {
    if (self->unk_78 != 0) {
        return self->unk_80 + self->unk_7c * 0x64;
    }
    return 0;
}
