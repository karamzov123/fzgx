#include "types.h"

typedef struct MovieModule {
    u8 pad[0xa8];
    void *field_a8;
} MovieModule;

extern void fn_12_758(void *, int *, int *, int *);

void fn_12_35084(MovieModule *self, int *out_a, int *out_b, int *out_c) {
    int value_a;
    int value_b;
    int value_c;

    fn_12_758(self->field_a8, &value_a, &value_b, &value_c);
    if (value_c == 1) {
        *out_c = 1;
    } else {
        *out_c = 0;
    }
    *out_a = value_a;
    *out_b = value_b;
}
