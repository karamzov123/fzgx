#include "types.h"

extern void fn_1_D550C(void *);

// fn_1_D5958: iterate through 4 structures, calling fn_1_D550C on each.
void fn_1_D5958(void *arg) {
    void *ptr = arg;
    int i = 0;
    for (; i < 4; i++) {
        fn_1_D550C((char *)ptr + 0xc07c);
        ptr = (char *)ptr + 0x30;
    }
}
