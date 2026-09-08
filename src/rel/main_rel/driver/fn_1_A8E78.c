#include "types.h"

extern void fn_1_A9420(int);
extern void fn_1_A948C(int);

// fn_1_A8E78: Take an argument, call fn_1_A9420(0), then fn_1_A948C with original arg.

void fn_1_A8E78(int arg) {
    fn_1_A9420(0);
    fn_1_A948C(arg);
}
