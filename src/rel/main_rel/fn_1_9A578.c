#include "types.h"

extern s16 lbl_1_data_2A7E0;
extern void fn_1_10302C(void);

typedef void (*fn_1_9A578_callback)(void);

struct fn_1_9A578_entry {
    u8 pad[8];
    fn_1_9A578_callback callback;
    u8 rest[0x18];
};

extern struct fn_1_9A578_entry lbl_1_data_2ABAC[];

void fn_1_9A578(void) {
    if (lbl_1_data_2A7E0 > 0) {
        fn_1_10302C();
        lbl_1_data_2ABAC[lbl_1_data_2A7E0].callback();
    }
}
