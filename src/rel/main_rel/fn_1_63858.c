#include "types.h"

extern void fn_1_862D4(s16, void *);
extern void lbl_8006DCA4(void);
extern f32 lbl_1_rodata_2B2C[145];
extern int fn_1_54E34(void *, f32);
extern void *fn_1_5448C(void *);
extern void *fn_1_548AC(int);
extern void fn_1_638E8(void);
extern void fn_1_5489C(void *, void *);

struct LocalData {
    u8 data[0x10];
};

struct Event {
    u8 data[4];
    void (*callback)(void);
    void *owner;
};

struct Object {
    u8 data[0x18];
    s16 value;
};

void fn_1_63858(struct Object *object) {
    struct LocalData local;
    struct Event *event;
    void *target;

    fn_1_862D4(object->value, &local);
    lbl_8006DCA4();
    if (fn_1_54E34(&local, lbl_1_rodata_2B2C[0]) != 0) {
        target = fn_1_5448C(&local);
        event = (struct Event *)fn_1_548AC(0xc);
        if (event != 0) {
            event->callback = fn_1_638E8;
            event->owner = object;
            fn_1_5489C(target, event);
        }
    }
}
