#include "types.h"

extern u32 Link(u32, u32, u32);

void OSLink(u32 arg0, u32 arg1) {
    Link(arg0, arg1, 0);
}
