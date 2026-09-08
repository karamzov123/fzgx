#include "types.h"

extern void fn_12_48A0(void *arg);
extern void fn_12_354C(void *arg);
extern u32 lbl_12_bss_10[300];

typedef struct MovieModule {
    u32 field_00;
    u8 pad_04[0x1c];
    void *field_20;
    u8 pad_24[8];
    void *field_2c;
} MovieModule;

void fn_12_30E8(MovieModule *movie) {
    if (movie != 0) {
        void *arg0 = movie->field_20;
        void *arg1 = movie->field_2c;

        movie->field_00 = 0;
        fn_12_48A0(arg0);
        fn_12_354C(arg1);
        lbl_12_bss_10[0]--;
    }
}
