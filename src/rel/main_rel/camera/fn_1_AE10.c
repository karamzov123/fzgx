#include "types.h"

extern void fn_1_AEB8(void);

typedef struct {
    u8 pad[0xa4];
    u16 field_a4;
} Camera;

void fn_1_AE10(Camera *camera) {
    camera->field_a4 = 0;
    fn_1_AEB8();
}
