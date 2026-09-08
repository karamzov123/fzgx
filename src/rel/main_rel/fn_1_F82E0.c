#include "types.h"

extern int fn_1_14D6D8(s16 value);
extern void fn_1_F83F0(s16 value);

void fn_1_F82E0(void) {
    s16 i;

    for (i = 0; i < 75; i++) {
        if (fn_1_14D6D8(i) & 0x20000000) {
            fn_1_F83F0(i);
        }
    }
}
