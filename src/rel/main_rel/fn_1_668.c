#include "types.h"

struct ArenaData {
    u32 *arena_lo;
    u32 arena_lo_size;
    u32 *aligned_lo;
    u32 aligned_lo_size;
};

extern struct ArenaData lbl_1_data_8;

extern u32 OSGetArenaLo(void);
extern u32 OSGetArenaHi(void);
extern void OSSetArenaLo(u32 arena_lo);

void fn_1_668(void) {
    u32 arena_lo;
    u32 aligned_lo;

    arena_lo = OSGetArenaLo();
    OSGetArenaHi();
    *lbl_1_data_8.arena_lo = arena_lo;
    aligned_lo = (arena_lo + lbl_1_data_8.arena_lo_size + 0x1f) & ~0x1f;
    *lbl_1_data_8.aligned_lo = aligned_lo;
    aligned_lo = (aligned_lo + lbl_1_data_8.aligned_lo_size + 0x1f) & ~0x1f;
    OSSetArenaLo(aligned_lo);
}
