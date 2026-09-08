#include "types.h"

typedef struct {
    u8 pad[0x3c];
    void *object;
} FE784Data;

typedef struct {
    u8 pad[0x10];
    s32 active;
} FE784Object;

extern FE784Data lbl_1_data_2A7E0;
extern void fn_1_9AD88(FE784Data *);
extern void fn_1_10069C(FE784Object *);
extern void fn_1_FF038(FE784Object *);

void fn_1_FE784(void) {
    FE784Object *object;

    object = (FE784Object *)lbl_1_data_2A7E0.object;
    fn_1_9AD88(&lbl_1_data_2A7E0);
    if (object->active != 0) {
        fn_1_10069C(object);
    }
    fn_1_FF038(object);
}
