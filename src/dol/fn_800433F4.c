#include "types.h"

typedef struct AdxStream {
    u8 pad_0[0x9A];
    s16 unk_9A;
    u8 pad_9C[0x14];
    void* unk_B0;
} AdxStream;

extern void (*lbl_8017A284[])(void*, void*);

void fn_800433F4(AdxStream* p, void* arg) {
    if (p->unk_B0 != 0) {
        lbl_8017A284[0](p->unk_B0, arg);
    }
}
