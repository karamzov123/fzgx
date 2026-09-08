#include "types.h"

extern void lbl_8006DCA4(void *object);
extern s32 fn_1_54E34(void *object, f32 value);
extern void *fn_1_5448C(void *object);
extern void *fn_1_548AC(s32 size);
extern void fn_1_5489C(void *object, void *event);
extern void fn_1_65748(void);

typedef struct {
    u32 pad_00;
    void (*vtable)(void);
    void *owner;
} Event;

typedef struct {
    u8 pad_00[0x28];
    f32 value;
    u8 pad_2c[0x10];
    u8 embedded[1];
} Object;

void fn_1_656C8(Object *object) {
    void *result;
    Event *event;

    lbl_8006DCA4(object);
    if (fn_1_54E34(&object->embedded[0], object->value) != 0) {
        result = fn_1_5448C(&object->embedded[0]);
        event = (Event *)fn_1_548AC(0xc);
        if (event != 0) {
            event->vtable = fn_1_65748;
            event->owner = object;
            fn_1_5489C(result, event);
        }
    }
}
