#include "types.h"

extern void *memset(void *, int, unsigned int);

typedef struct Fn_1_235C8 {
    unsigned char *field00;
    unsigned char *field04;
    unsigned char data[0xb8];
} Fn_1_235C8;

void fn_1_235C8(Fn_1_235C8 *self) {
    if (self != 0) {
        memset(self, 0, 0xc0);
        self->field00 = &self->data[0];
        self->field04 = &self->data[0x5c];
    }
}

