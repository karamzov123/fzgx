#include "types.h"

extern int fn_1_58C4(void);
extern void fn_1_5948(int);
extern void fn_1_627C(int);

// Calls initialization, then loops calling two functions for each item
void fn_1_1539D0(void) {
    int count = fn_1_58C4();
    int i;
    for (i = 0; i < count; i++) {
        fn_1_5948(i);
        fn_1_627C(i);
    }
}
