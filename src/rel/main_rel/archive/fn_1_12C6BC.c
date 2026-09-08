#include "types.h"

extern void fn_1_14DBCC(void *arg);

void fn_1_12C6BC(void *arg) {
    u8 *p;
    s16 i;

    p = (u8 *)arg;
    i = 0;
    while (i < 9) {
        fn_1_14DBCC(p);
        p += 0xa20;
        i++;
    }
}
