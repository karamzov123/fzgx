#include "types.h"

extern f32 lbl_1_rodata_29AC[5];
extern u32 lbl_1_data_1D628;
extern f32 lbl_1_rodata_2978[4];

struct Fn15D88C {
    u8 _pad08[0x08];
    u32 flags;
    u8 _pad0c[0x1c];
    f32 value28;
    u8 _pad2c[0x08];
    u32 enabled;
    u8 _pad38[0x1c];
    u16 value54;
    u16 value56;
    u16 value58;
    u8 _pad5a[0x5a];
    f32 value_b4;
};

void fn_1_5D88C(struct Fn15D88C *self) {
    self->value_b4 = lbl_1_rodata_29AC[0];
    if (self->enabled != 0) {
        f32 value28;
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        value28 = lbl_1_rodata_2978[0];
        self->value54 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        self->value56 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        lbl_1_data_1D628 = lbl_1_data_1D628 * 1103515245 + 12345;
        self->value58 = (u16)((lbl_1_data_1D628 >> 16) & 0x7fff);
        self->value28 = value28;
    }
    self->flags |= 0x40000000;
}
