#include "types.h"

extern u8 lbl_1_bss_84454[3124];

typedef struct {
    u8 pad14[0x14];
    int field14;
    u8 pad18[0x5c];
    int field74;
    u8 pad78[0x5c];
    int fieldd4;
    u8 padd8[0x5c];
    int field134;
    u8 pad138[0x5c];
    int field194;
    u8 pad198[0x5c];
    int field1f4;
    u8 pad1f8[0x78];
} State;

void fn_1_FB9DC(int index) {
    State *state = (State *)lbl_1_bss_84454 + (index & 0xff);

    state->field14 = 0;
    state->field134 = 0;
    state->field74 = 0;
    state->field194 = 0;
    state->fieldd4 = 0;
    state->field1f4 = 0;

    switch (index & 0xff) {
    case 0:
        state->field14 = 1;
        state->field134 = 1;
        break;
    case 1:
        state->field134 = 1;
        state->field194 = 1;
        break;
    case 2:
        state->field134 = 1;
        state->field1f4 = 1;
        break;
    case 3:
        state->field134 = 1;
        state->field1f4 = 1;
        break;
    case 4:
        state->field134 = 1;
        state->field194 = 1;
        break;
    default:
        break;
    }
}
