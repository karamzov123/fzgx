#include "types.h"

typedef struct {
    u8 pad[0x90];
    u32 field;
} Fn800452FCObject;

u32 fn_800452FC(Fn800452FCObject *object) {
    return object->field;
}
