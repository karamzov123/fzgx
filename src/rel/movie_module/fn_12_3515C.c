#include "types.h"

typedef struct MovieModule {
    u8 pad[0xa8];
    void *movie;
} MovieModule;

extern void fn_12_7C4(void *movie);

void fn_12_3515C(MovieModule *module) {
    fn_12_7C4(module->movie);
}
