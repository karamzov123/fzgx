#include "types.h"

typedef struct MovieObject {
    u8 pad[0x58];
    s32 state;
} MovieObject;

extern u32 lbl_12_bss_1CC58;
extern u32 lbl_12_bss_1CC5C;
extern s32 lbl_12_bss_1B9DC[52];
extern MovieObject *fn_12_38DBC(void);
extern s32 fn_12_34C44(s32 *state);
extern void fn_12_2CD6C(void);

void fn_12_3ABE0(void) {
    MovieObject *movie;

    lbl_12_bss_1CC58++;
    lbl_12_bss_1CC5C++;

    if (lbl_12_bss_1B9DC[0] == 1) {
        movie = fn_12_38DBC();
        if (fn_12_34C44(&movie->state)) {
            if (lbl_12_bss_1B9DC[0] == 1) {
                fn_12_2CD6C();
            }
            movie->state = 0;
        }
    }
}
