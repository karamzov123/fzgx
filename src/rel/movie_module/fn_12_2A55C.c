#include "types.h"

extern int fn_12_24990(void *arg0, void *arg1);
extern int fn_12_24A88(int arg0, u32 arg1);
extern int fn_12_2ABF0(void *arg0);

int fn_12_2A55C(void *arg0, int *arg1) {
    *arg1 = 0;
    if (fn_12_24990(arg0, arg1)) {
        return fn_12_24A88(0, 0xff000182);
    }
    *arg1 = fn_12_2ABF0(arg0);
    return 0;
}
