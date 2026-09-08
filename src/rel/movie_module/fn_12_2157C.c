#include "types.h"

typedef struct MovieModule {
    char _pad[0x1b74];
    void *movie;
} MovieModule;

extern void fn_8004BF0C(void *movie);

void fn_12_2157C(MovieModule *self) {
    fn_8004BF0C(*(void **)self->movie);
}
