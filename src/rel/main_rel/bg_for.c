#include "types.h"

/* fzgx:begin fn_1_DCF54 */
typedef struct Vec3 {
    u32 x;
    u32 y;
    u32 z;
} Vec3;

typedef struct State {
    Vec3 a;
    u8 pad[12];
    Vec3 b;
    Vec3 c;
} State;

extern State lbl_1_bss_7ADE8;

void fn_1_DCF54(Vec3 *a, Vec3 *b, Vec3 *c) {
    lbl_1_bss_7ADE8.a = *a;
    lbl_1_bss_7ADE8.b = *b;
    lbl_1_bss_7ADE8.c = *c;
}
/* fzgx:end fn_1_DCF54 */
