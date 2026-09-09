#include "types.h"

typedef struct Fn80058380Object Fn80058380Object;
typedef void (*Fn80058380Callback)(void *, int);

struct Fn80058380Object {
    u8 pad_0x00[0x0C];
    int value_0x0C;
    int value_0x10;
    u8 pad_0x14[0x24];
    Fn80058380Callback callback;
    void *callback_context;
};

int fn_80058380(Fn80058380Object *object, int mode) {
    if (mode == 1) {
        return object->value_0x0C;
    }
    if (mode == 0) {
        return object->value_0x10;
    }
    if (object->callback) {
        object->callback(object->callback_context, -3);
    }
    return 0;
}
