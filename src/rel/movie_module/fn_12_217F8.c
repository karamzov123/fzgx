#include "types.h"

extern int fn_12_2D73C(int arg0, int arg1);
extern int fn_12_2F210(int arg0, int arg1, int arg2, int arg3, int arg4);

int fn_12_217F8(int arg0) {
    int result;
    int value;

    if (fn_12_2D73C(arg0, 6) == 0) {
        return 0;
    }

    value = fn_12_2F210(arg0, 3, 6, 0, 0);
    result = 0;
    if (value != 0) {
        result = value;
    }
    return result;
}
