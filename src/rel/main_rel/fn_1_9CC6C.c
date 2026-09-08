#include "types.h"

typedef struct {
    u8 pad[0x30];
    u32 value30;
    u32 value34;
} GlobalState;

extern void fn_1_9DB04(void);
extern void fn_1_9C724(void);
extern void fn_1_9BBE4(void *, s32, s32, s32);
extern u32 lbl_1_data_2A7E0[62];
extern GlobalState *lbl_801A66CC;

void fn_1_9CC6C(void *arg0, s32 arg1) {
    if (arg0 != 0) {
        fn_1_9DB04();
        fn_1_9C724();
        fn_1_9BBE4(arg0, 0, arg1 - 1, 0);
        lbl_801A66CC->value30 = lbl_1_data_2A7E0[26];
        lbl_801A66CC->value34 = lbl_1_data_2A7E0[27];
    }
}
