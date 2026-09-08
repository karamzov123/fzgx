#include "types.h"

typedef struct Fn12_39E54_Object {
    u32 unk0;
    u32 unk4;
} Fn12_39E54_Object;

extern char lbl_12_rodata_2940[44];
extern void fn_12_34B88(char *, ...);
extern void fn_12_34150(void);

void fn_12_39E54(Fn12_39E54_Object *object) {
    s32 value;

    if (object == 0) {
        value = 0;
    } else {
        value = object->unk4;
    }

    if (value == 0) {
        fn_12_34B88(lbl_12_rodata_2940);
    } else {
        fn_12_34150();
    }
}
