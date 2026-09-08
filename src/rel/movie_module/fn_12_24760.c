#include "types.h"

extern void *fn_12_3310C(void);
extern int fn_12_32EAC(void *movie, u32 *result);
extern void fn_12_330E0(void *movie);

u32 fn_12_24760(void) {
    void *movie;
    u32 result;

    movie = fn_12_3310C();
    if (movie == 0) {
        return 0;
    }
    if (fn_12_32EAC(movie, &result) == 0) {
        result = 0;
    }
    fn_12_330E0(movie);
    return result;
}
