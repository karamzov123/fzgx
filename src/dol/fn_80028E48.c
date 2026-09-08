#include "types.h"

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 level);

typedef struct {
    u8 pad0[8];
    u32 field8;
} Fn80028E48Data;

Fn80028E48Data *fn_80028E48(Fn80028E48Data *data) {
    u32 level;

    level = OSDisableInterrupts();
    data->field8 |= 2;
    OSRestoreInterrupts(level);
    return data;
}
