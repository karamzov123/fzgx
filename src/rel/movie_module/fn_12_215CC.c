#include "types.h"

typedef struct MovieModule {
    u8 pad_0[0x1b74];
    void *field_1b74;
} MovieModule;

extern void fn_8004BFAC(void *);

void fn_12_215CC(MovieModule *movie) {
    fn_8004BFAC(*(void **)movie->field_1b74);
}
