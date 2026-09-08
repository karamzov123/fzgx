#include "types.h"

typedef struct MovieModule {
    u8 pad[0x40];
    void *movie;
} MovieModule;

extern void fn_12_21B70(void *movie);

void fn_12_34150(MovieModule *module) {
    fn_12_21B70(module->movie);
}
