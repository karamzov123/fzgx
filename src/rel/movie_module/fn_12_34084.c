#include "types.h"

extern int fn_12_3A36C(void *);
extern char lbl_12_rodata_1280[46];
extern void fn_12_34B88(char *, ...);

typedef struct MovieModule {
    u8 pad[0x54];
    void *movie;
} MovieModule;

void *fn_12_34084(MovieModule *module) {
    if (fn_12_3A36C(module) == 0) {
        fn_12_34B88(lbl_12_rodata_1280);
        return 0;
    }
    return module->movie;
}
