#include "types.h"

extern int fn_12_2D73C(void *arg, int value);
extern int fn_12_2F210(void *arg, int a, int b, int c, int d);

int fn_12_21864(void *arg) {
    int temp;
    int result;

    if (fn_12_2D73C(arg, 6) == 0) {
        return 0;
    }

    temp = fn_12_2F210(arg, 3, 5, 0, 0);
    result = 0;
    if (temp != 0) {
        result = temp;
    }

    return result;
}
