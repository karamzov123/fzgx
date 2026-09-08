#include "types.h"

typedef struct Fn80006DFCObject {
    s32 state;
    u8 pad[0x30];
    void *value;
} Fn80006DFCObject;

void *fn_80006DFC(Fn80006DFCObject *object) {
    if (object->state != 1) {
        object = object->value;
    }
    return object;
}
