#include "types.h"

typedef struct Movie Movie;
typedef struct MovieVtable MovieVtable;

struct MovieVtable {
    char _pad[8];
    int (*callback)(Movie *, void *);
};

struct Movie {
    char _pad_0000[0x1c84];
    MovieVtable *vtable;
};

extern int fn_12_24990(void);
extern void fn_12_24A88(int, int);
extern int fn_12_2D73C(Movie *, int);

int fn_12_21AE4(Movie *movie, void *arg) {
    if (fn_12_24990() != 0) {
        fn_12_24A88(0, 0xff0001a2);
        return 0;
    }

    if (fn_12_2D73C(movie, 6) == 0) {
        return 0;
    }

    return movie->vtable->callback(movie, arg);
}
