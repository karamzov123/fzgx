#include "types.h"

struct fn_80058EE4_State {
    u32 first;
    u32 second;
};

extern struct fn_80058EE4_State lbl_8018FF70[];

void fn_80058EE4(u32 first, u32 second) {
    struct fn_80058EE4_State* state = &lbl_8018FF70[0];
    state->first = first;
    state->second = second;
}
