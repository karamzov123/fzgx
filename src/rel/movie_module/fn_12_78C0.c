#include "types.h"

typedef struct MovieModule MovieModule;

struct MovieModule {
    u8 _pad_000[0x2a8];
    void (*update)(MovieModule *);
    u8 _pad_2ac[0x5c];
    s32 frame_count;
    u8 _pad_30c[0x8];
    s32 field_314;
};

void fn_12_78C0(MovieModule *movie, s32 frame) {
    s32 end;
    s32 current;
    void (*update)(MovieModule *);

    end = movie->frame_count;
    current = end - frame + 1;
    movie->field_314 = 0;
    update = movie->update;
    while (current < end) {
        movie->frame_count = current;
        update(movie);
        current++;
    }
    movie->frame_count = end;
}
