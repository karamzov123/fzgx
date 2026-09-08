#include "types.h"

typedef struct MovieObject {
    s32 state;
    u8 pad[0x10];
    void *movie;
} MovieObject;

typedef struct MovieModule {
    u8 pad[0x18];
    void (*callback)(void *, void *);
} MovieModule;

extern u32 lbl_12_data_E90[2];

void fn_12_3B540(MovieObject *self) {
    MovieModule *module = (MovieModule *)lbl_12_data_E90[0];
    s32 result;

    if (module == 0) {
        result = 0;
    } else if (self->state != 1) {
        result = 0;
    } else if (self->movie == 0) {
        result = 0;
    } else {
        result = 1;
    }

    if (result == 1) {
        void *movie = self->movie;

        if (module != 0 && module->callback != 0) {
            module->callback(movie, module);
        }
    }
}
