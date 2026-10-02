#include "types.h"

typedef struct {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} Quad;

extern const Quad lbl_12_rodata_A08;

typedef struct {
    u8 pad[0x34];
    Quad q;
} MovieFieldSet;

void fn_12_2013C(MovieFieldSet *p) {
    p->q = lbl_12_rodata_A08;
}
