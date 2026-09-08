#include "types.h"

extern int fn_12_24990(void *arg0, int arg1);
extern int fn_12_24A88(int arg0, u32 arg1);
extern int fn_12_CAE4(int arg0, int arg1, int arg2);

typedef struct MovieModule {
    unsigned char _pad[0x1b30];
    void *field_1b30;
} MovieModule;

int fn_12_2A5C8(MovieModule *arg0, int arg1, int arg2) {
    int result;

    if (arg0 == 0) {
        result = 0;
    } else if (fn_12_24990(arg0, arg1)) {
        return fn_12_24A88(0, 0xff000181);
    } else {
        result = *(int *)arg0->field_1b30;
    }

    if (arg1 == 5) {
        arg2 = 0;
    }
    if (fn_12_CAE4(result, arg1, arg2)) {
        return fn_12_24A88((int)arg0, 0xff000f12);
    }
    return 0;
}
