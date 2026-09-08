#include "types.h"

typedef struct {
    u8 pad20[0x20];
    void *field20;
} Fn1_61E60Node;

typedef struct {
    u8 pad38[0x38];
    Fn1_61E60Node *field38;
} Fn1_61E60Object;

extern void *lbl_801A6410;
extern u8 lbl_1_data_1D62C[148];

extern void fn_1_4730(void *arg0, void *arg1, int arg2, u8 *arg3, int arg4);
extern void fn_1_46B4(void *arg0, void *arg1, u8 *arg2, int arg3);

int fn_1_61E60(Fn1_61E60Object *object) {
    Fn1_61E60Node *node = object->field38;

    if (node != 0) {
        fn_1_4730(lbl_801A6410, node->field20, 1, lbl_1_data_1D62C, 0x17D5);
        node->field20 = 0;
        fn_1_46B4(lbl_801A6410, object->field38, lbl_1_data_1D62C, 0x17D8);
        object->field38 = 0;
    }

    return 1;
}
