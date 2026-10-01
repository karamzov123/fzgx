#include "types.h"

u32 fn_12_A2C4(const u8 *data) {
    s32 value = (data[0] << 8) | data[1];
    u32 result;

    value <<= 8;
    value |= data[2];
    value <<= 8;
    value |= data[3];

    if (value == 0x100) {
        result = 4;
    } else if (value == 0x101) {
        result = 3;
    } else if (value > 0x101 && value <= 0x1AF) {
        result = 1;
    } else if (value == 0x1B2) {
        result = 0x20;
    } else if (value == 0x1B3) {
        result = 0x40;
    } else if (value == 0x1B5) {
        result = 0x10;
    } else if (value == 0x1B7) {
        result = 0x80;
    } else if (value == 0x1B8) {
        result = 8;
    } else {
        result = 0;
    }
    return result;
}
