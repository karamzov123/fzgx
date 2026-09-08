#include "types.h"

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32);

typedef struct Fn800231C4Work {
    u8 _pad0[0x1c];
    u32 flags;
    u8 _pad1[0x14e];
    s16 field_16e;
    u8 _pad2[4];
    s16 field_174;
    s16 field_176;
    s16 field_178;
    s16 field_17a;
} Fn800231C4Work;

void fn_800231C4(Fn800231C4Work *work) {
    u32 interrupts = OSDisableInterrupts();
    work->field_16e = 1;
    work->field_17a = 0;
    work->field_178 = 0;
    work->field_176 = 0;
    work->field_174 = 0;
    work->flags = work->flags & ~0x40;
    work->flags = work->flags | 0x20;
    OSRestoreInterrupts(interrupts);
}
