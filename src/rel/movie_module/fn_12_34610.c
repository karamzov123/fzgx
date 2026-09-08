#include "types.h"

typedef struct MovieModule {
    u8 _pad[0x4c];
    void *movie;
} MovieModule;

extern void fn_80056BE0(void *movie);

void fn_12_34610(MovieModule *module) {
    fn_80056BE0(module->movie);
}
