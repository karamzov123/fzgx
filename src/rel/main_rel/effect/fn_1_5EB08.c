#include "types.h"

typedef struct FnObj {
    u8 pad18[0x18];
    s16 value;
    u8 pad3c[0x22];
    f32 field3c;
} FnObj;

typedef struct FnLocal {
    u8 data[0x10];
} FnLocal;

typedef struct FnNode {
    u8 pad4[4];
    void (*callback)(void);
    FnObj *object;
} FnNode;

extern s32 fn_1_862D4(s16 value, FnLocal *local);
extern void lbl_8006DCA4(void);
extern f32 lbl_1_rodata_2AF4[14];
extern s32 fn_1_54E34(void *field, f32 value);
extern FnNode *fn_1_5448C(FnLocal *local);
extern FnNode *fn_1_548AC(s32 size);
extern void fn_1_5EB98(void);
extern void fn_1_5489C(FnNode *node, FnNode *other);

void fn_1_5EB08(FnObj *object) {
    FnLocal local;
    FnNode *node;
    FnNode *allocated;

    fn_1_862D4(object->value, &local);
    lbl_8006DCA4();
    if (fn_1_54E34(&object->field3c, lbl_1_rodata_2AF4[0])) {
        node = fn_1_5448C(&local);
        allocated = fn_1_548AC(0xc);
        if (allocated != 0) {
            allocated->callback = fn_1_5EB98;
            allocated->object = object;
            fn_1_5489C(node, allocated);
        }
    }
}
