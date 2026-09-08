#include "types.h"

typedef struct Camera {
    u8 pad_00[0x78];
    s16 unk_78;
    u8 pad_7A[0x2A];
    s16 unk_A4;
} Camera;

extern void fn_1_AEB8(Camera*);
extern void fn_1_AFC8(Camera*);
extern s16 lbl_1_bss_1008;

void fn_1_AE38(Camera* self) {
    if (self->unk_78 == 0) {
        self->unk_A4++;
        if (self->unk_A4 >= lbl_1_bss_1008) {
            self->unk_A4 = 0;
        }
        fn_1_AEB8(self);
    }
    fn_1_AFC8(self);
    if (self->unk_78 != 0) {
        self->unk_78--;
    }
}
