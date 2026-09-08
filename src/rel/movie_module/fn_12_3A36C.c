#include "types.h"

typedef struct Fn12_3A36C_Object {
    u32 unk0;
    u32 unk4;
} Fn12_3A36C_Object;

u32 fn_12_3A36C(Fn12_3A36C_Object *object) {
    if (object == 0) {
        return 0;
    }
    return object->unk4;
}
