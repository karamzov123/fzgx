#include "types.h"

typedef struct {
    u8 unk[0x18];
    s16 value;
    u8 unk1A[0x0E];
    f32 rate;
    u8 unk2C[0x10];
    u8 field3C[1];
} FZeroObject;

typedef struct {
    u8 unk0[4];
    void (*callback)(void);
    FZeroObject *owner;
} FZeroEvent;

extern void fn_1_862D4(s16 value, void *result);
extern void lbl_8006DCA4(void);
extern int fn_1_54E34(void *field, f32 rate);
extern void *fn_1_5448C(void *result);
extern void *fn_1_548AC(u32 size);
extern void fn_1_5FEBC(void);
extern void fn_1_5489C(void *callback, FZeroEvent *event);

void fn_1_5FE30(FZeroObject *object) {
    u8 result[8];
    void *callback;
    FZeroEvent *event;

    fn_1_862D4(object->value, result);
    lbl_8006DCA4();
    if (fn_1_54E34(object->field3C, object->rate) != 0) {
        callback = fn_1_5448C(result);
        event = (FZeroEvent *)fn_1_548AC(12);
        if (event != 0) {
            event->callback = fn_1_5FEBC;
            event->owner = object;
            fn_1_5489C(callback, event);
        }
    }
}
