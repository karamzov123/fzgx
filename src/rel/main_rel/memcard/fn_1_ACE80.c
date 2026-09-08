#include "types.h"

extern void fn_8002FB04(u8, void*, u32);

typedef struct {
    u8 field_0;
    u8 pad_1[0x23];
    void* field_24;
} SomeObject;

// fn_1_ACE80: writes 0x12c to pointed structure, then calls fn_8002FB04
void fn_1_ACE80(SomeObject* obj) {
    *(u16*)((u8*)obj->field_24 + 0x2) = 0x12c;
    fn_8002FB04(obj->field_0, (u8*)obj->field_24 + 0x98, 0);
}
