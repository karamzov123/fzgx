#include "types.h"

typedef struct AdxStream {
    u8 pad_0[0x9A];
    s16 unk_9A;
    u8 pad_9C[0x14];
    void* unk_B0;
} AdxStream;

extern void (*lbl_8017A280[])(AdxStream*);

void fn_800433A4(AdxStream* p) {
    lbl_8017A280[0](p);
}
