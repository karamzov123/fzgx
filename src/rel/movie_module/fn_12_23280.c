#include "types.h"

extern int fn_12_24990(void);
extern int fn_12_24A88(int arg0, int arg1);
extern void fn_12_2D7DC(void *arg0, int arg1, int arg2);

int fn_12_23280(void *arg0) {
    if (fn_12_24990()) {
        return fn_12_24A88(0, 0xff000161);
    }
    fn_12_2D7DC(arg0, 0x31, 1);
    return 0;
}
