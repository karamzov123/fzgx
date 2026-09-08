#include "types.h"

typedef void (*MovieCallback)(u32);

typedef struct MovieObject {
    MovieCallback callback0;
    union {
        u32 callback_arg0;
        MovieCallback callback1;
    };
    u32 callback_arg1;
    s32 state;
} MovieObject;

extern MovieObject *lbl_12_bss_4DB8;

s32 fn_12_6D5C(MovieObject *movie, s32 value) {
    if (movie == 0) {
        movie = lbl_12_bss_4DB8;
        movie->callback_arg1 = value;
        if (value != 0) {
            if (movie->callback0 != 0) {
                movie->callback0(movie->callback_arg0);
            }
        }
    } else {
        movie->state = value;
        if (value != 0) {
            if (movie->callback1 != 0) {
                movie->callback1(movie->callback_arg1);
            }
        }
    }
    return value;
}
