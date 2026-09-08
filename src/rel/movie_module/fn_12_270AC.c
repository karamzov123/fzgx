#include "types.h"

typedef struct MovieObject {
    u8 pad_0038[0x38];
    int field_0038;
    u8 pad_003c[0x1af4];
    void *field_1b30;
    u8 pad_1b34[0x38c];
    void *field_1ec0;
} MovieObject;

extern int fn_12_2D73C(MovieObject *, int);
extern int fn_12_26D68(MovieObject *, void *);
extern void *fn_12_CF2C(void);
extern int fn_12_24A88(MovieObject *, int);
extern void fn_12_26D24(void);
extern int fn_12_A6E8(void *, void (*)(void), MovieObject *);
extern void fn_12_CBC8(void *);
extern void fn_12_CAE4(void *, int, int);

int fn_12_270AC(MovieObject *movie) {
    void *movie_data;
    void *movie_slot;
    int result;

    if (fn_12_2D73C(movie, 5) == 0) {
        return 0;
    }

    movie_data = &movie->field_1ec0;
    movie->field_1b30 = movie_data;
    result = fn_12_26D68(movie, movie_data);
    if (result != 0) {
        return result;
    }

    movie_slot = fn_12_CF2C();
    if (movie_slot == 0) {
        return fn_12_24A88(movie, (int)0xff000f0a);
    }

    if (fn_12_A6E8(movie_slot, fn_12_26D24, movie) != 0) {
        fn_12_CBC8(movie_slot);
        return fn_12_24A88(movie, (int)0xff000f0b);
    }

    fn_12_CAE4(movie_slot, 1, fn_12_2D73C(movie, 0));
    fn_12_CAE4(movie_slot, 2, fn_12_2D73C(movie, 1));
    fn_12_CAE4(movie_slot, 3, 0);
    fn_12_CAE4(movie_slot, 4, 0);
    fn_12_CAE4(movie_slot, 5, 0);
    fn_12_CAE4(movie_slot, 6, movie->field_0038);
    fn_12_CAE4(movie_slot, 7, 0x7fffffff);
    fn_12_CAE4(movie_slot, 8, 0);
    fn_12_CAE4(movie_slot, 9, 0);
    *(void **)movie_data = movie_slot;
    return 0;
}
