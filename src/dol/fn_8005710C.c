#include "types.h"

typedef struct {
    u8 pad_0x00[0x28];
    int value;
} Fn8005710CObject;

void fn_8005710C(Fn8005710CObject *object, int value) {
    object->value = value;
}
