#include "types.h"

struct MovieModule {
    char pad_44[0x44];
    int flag_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
};

extern int fn_12_24990(struct MovieModule *module);
extern int fn_12_24A88(int value, u32 code);
extern void fn_12_2DFF0(struct MovieModule *module, int value);
extern int fn_12_2F210(struct MovieModule *module, int value, int size, int data, int flags);

int fn_12_2AF5C(struct MovieModule *module, int flag) {
    int result;
    int state;
    int v;
    int cur;

    if (fn_12_24990(module)) {
        return fn_12_24A88(0, 0xff000142);
    }

    cur = module->field_50;
    if (flag == 0) {
        if (cur == 0) {
            return 0;
        }
        state = 0;
    } else if (cur == 0) {
        state = 1;
    } else {
        state = 2;
    }

    result = 0;
    module->field_50 = flag;

    switch (state) {
    case 2:
        if (module->field_48 == 4) {
            switch (module->field_4C) {
            case 3:
                /* shared action block: the two accepted field_4C values dispatch here */
                goto action2;
            default:
                switch (module->field_4C) {
                case 4:
                    /* shared action block: the second accepted field_4C value dispatches here */
                    goto action2;
                }
            }
            break;
        action2:
            fn_12_2DFF0(module, 2);
            v = fn_12_2F210(module, 7, 8, 2, 0);
            result = 0;
            if (v != 0) {
                result = v;
            }
        }
        break;
    case 1:
        if (module->field_54++ == 0) {
            switch (module->field_4C) {
            case 3:
                /* shared action block: the two accepted field_4C values dispatch here */
                goto action1;
            default:
                switch (module->field_4C) {
                case 4:
                    /* shared action block: the second accepted field_4C value dispatches here */
                    goto action1;
                }
            }
            break;
        action1:
            fn_12_2DFF0(module, 1);
            v = fn_12_2F210(module, 7, 8, 1, 0);
            result = 0;
            if (v != 0) {
                result = v;
            }
        }
        break;
    case 0:
        if (--module->field_54 == 0) {
            switch (module->field_4C) {
            case 3:
                /* shared action block: the two accepted field_4C values dispatch here */
                goto action0;
            default:
                switch (module->field_4C) {
                case 4:
                    /* shared action block: the second accepted field_4C value dispatches here */
                    goto action0;
                }
            }
            break;
        action0:
            fn_12_2DFF0(module, 0);
            v = fn_12_2F210(module, 7, 8, 0, 0);
            result = 0;
            if (v != 0) {
                result = v;
            }
        }
        break;
    }

    module->flag_44 = 1;
    return result;
}
