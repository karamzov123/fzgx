#include "types.h"

typedef void (*fn_1_9A770_callback)(void);

struct fn_1_9A770_data {
    u8 pad[0x38];
    fn_1_9A770_callback callback;
};

extern struct fn_1_9A770_data lbl_1_data_2A7E0;

void fn_1_9A770(void) {
    if (lbl_1_data_2A7E0.callback != 0) {
        lbl_1_data_2A7E0.callback();
    }
}
