#include "types.h"

typedef struct MovieInner {
    void *value;
} MovieInner;

typedef struct Movie {
    u8 pad_0000[0x1b74];
    MovieInner *inner;
} Movie;

extern const f64 lbl_12_rodata_A70;
extern const f32 lbl_12_rodata_A68;
extern const f32 lbl_12_rodata_A64;
extern const f32 lbl_12_rodata_A6C;

extern f64 fn_80088558(Movie *movie, f64 value);
extern void fn_8004BBC8(void *value, int minutes, int seconds);

void fn_12_214B0(Movie *movie, int value) {
    f32 elapsed;
    f32 scaled;
    int minutes;
    int seconds;

    elapsed = (f32)fn_80088558(movie, (f64)value);
    scaled = elapsed - lbl_12_rodata_A68;
    scaled = lbl_12_rodata_A64 * scaled;
    minutes = (int)(lbl_12_rodata_A6C * scaled);
    seconds = (int)scaled - minutes * 100;
    fn_8004BBC8(movie->inner->value, minutes, seconds);
}
