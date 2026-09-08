#include "types.h"

typedef struct Fn12_3A384_Object {
    u8 _pad[0x44];
    u32 value;
} Fn12_3A384_Object;

extern void fn_12_34FA8(u32 value);
extern void fn_12_3428C(Fn12_3A384_Object *object, u32 value);

void fn_12_3A384(Fn12_3A384_Object *object, u32 value) {
    fn_12_34FA8(object->value);
    fn_12_3428C(object, value);
}
