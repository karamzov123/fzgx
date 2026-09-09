#include "types.h"

struct fn_80058ED4_State {
    u32 first;
    u32 second;
};

extern struct fn_80058ED4_State lbl_8018FF68[];

void fn_80058ED4(u32 first, u32 second) {
    struct fn_80058ED4_State* state = &lbl_8018FF68[0];
    state->first = first;
    state->second = second;
}
