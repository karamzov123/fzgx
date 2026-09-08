#include "types.h"

typedef struct MovieModule {
    u8 unk_000[0x2a8];
    void (*callback)(struct MovieModule *);
    u8 unk_2ac[0x5c];
    s32 current_frame;
    u8 unk_30c[0x08];
    s32 field_314;
} MovieModule;

void fn_12_11254(MovieModule *movie, s32 frame) {
    s32 end_frame;
    s32 current_frame;
    void (*callback)(MovieModule *);

    end_frame = movie->current_frame;
    movie->field_314 = 0;
    current_frame = end_frame - frame + 1;
    callback = movie->callback;
    while (current_frame < end_frame) {
        movie->current_frame = current_frame;
        callback(movie);
        current_frame += 1;
    }
    movie->current_frame = end_frame;
}
