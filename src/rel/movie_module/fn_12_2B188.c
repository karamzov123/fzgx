#include "types.h"

extern int fn_12_24990(void);
extern void fn_12_24A88(int arg0, u32 arg1);
extern void fn_12_226D0(void *arg0, void *arg1);

void fn_12_2B188(void *arg0, void *arg1) {
    if (fn_12_24990()) {
        fn_12_24A88(0, 0xff000139);
    } else {
        fn_12_226D0(arg0, arg1);
    }
}
