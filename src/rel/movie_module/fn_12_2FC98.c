#include "types.h"

typedef struct MovieModule {
    u8 _pad[0x1c48];
    int movie_id;
} MovieModule;

extern int fn_12_2D73C(MovieModule *, int);
extern int fn_12_2F1D0(MovieModule *, int);
extern int fn_12_21D30(MovieModule *, int);
extern int fn_12_2DABC(MovieModule *);
extern void fn_12_2F1E0(MovieModule *, int, int);
extern int fn_12_2F1F0(MovieModule *, int);
extern int fn_12_21D50(MovieModule *, int);
extern void fn_12_2F200(MovieModule *, int, int);

int fn_12_2FC98(MovieModule *movie_module) {
    int ready;

    if (fn_12_2D73C(movie_module, 5) == 0) {
        return 0;
    }

    if (fn_12_2F1D0(movie_module, 6) != 1 &&
        fn_12_21D30(movie_module, movie_module->movie_id) == 1) {
        if (fn_12_2D73C(movie_module, 15) == 0) {
            ready = 1;
        } else if (fn_12_2DABC(movie_module) == 0) {
            ready = 0;
        } else {
            ready = 1;
        }

        if (ready != 0) {
            fn_12_2F1E0(movie_module, 6, 1);
        }
    }

    if (fn_12_2F1F0(movie_module, 6) != 1 &&
        fn_12_21D50(movie_module, movie_module->movie_id) == 1) {
        fn_12_2F200(movie_module, 6, 1);
    }

    return 0;
}
