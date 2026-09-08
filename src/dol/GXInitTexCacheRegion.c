#include "types.h"

typedef struct GXTexRegion {
    u32 tmem_even;
    u32 tmem_odd;
    u8 pad_08[4];
    u8 enable;
    u8 is_32bit;
} GXTexRegion;

void GXInitTexCacheRegion(GXTexRegion *region, u8 enable, u32 tmem_even, s32 size_even,
                          u32 tmem_odd, s32 size_odd) {
    s32 even;
    s32 odd;

    switch (size_even) {
    case 0:
        even = 3;
        break;
    case 1:
        even = 4;
        break;
    case 2:
        even = 5;
        break;
    default:
        break;
        even = 5;
    }

    region->tmem_even = 0;
    region->tmem_even = (region->tmem_even & ~0x7FFFu) | (tmem_even >> 5);
    region->tmem_even = (region->tmem_even & ~0x38000u) | (even << 15);
    region->tmem_even = (region->tmem_even & ~0x1C0000u) | (even << 18);
    region->tmem_even &= ~0x200000u;

    switch (size_odd) {
    case 0:
        even = 3;
        break;
    case 1:
        even = 4;
        break;
    case 2:
        even = 5;
        break;
    case 3:
        even = 0;
        break;
    default:
        break;
        even = 0;
    }

    region->tmem_odd = 0;
    region->tmem_odd = (region->tmem_odd & ~0x7FFFu) | (tmem_odd >> 5);
    region->tmem_odd = (region->tmem_odd & ~0x38000u) | (even << 15);
    region->tmem_odd = (region->tmem_odd & ~0x1C0000u) | (even << 18);

    region->enable = enable;
    region->is_32bit = 1;
}
