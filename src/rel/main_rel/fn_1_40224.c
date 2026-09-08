#include "types.h"

extern int fn_1_F9FEC(void);
extern int fn_1_FA180(int, int);

int fn_1_40224(void) {
    int result;

    if (fn_1_F9FEC() != 0) {
        return 1;
    }
    if (fn_1_FA180(2, 0) == 0) {
        return 0;
    }
    if (fn_1_FA180(2, 1) == 0) {
        return 0;
    }
    result = fn_1_FA180(2, 2);
    return result != 0;
}
