#include "types.h"

typedef struct MovieModule {
    u8 pad_0[0x1b74];
    void **movie;
} MovieModule;

extern void fn_8004BF68(void *);

void fn_12_215A4(MovieModule *this) {
    fn_8004BF68(*this->movie);
}
