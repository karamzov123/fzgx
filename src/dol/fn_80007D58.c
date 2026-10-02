#include "types.h"

typedef union {
    f32 f;
    u32 u;
} FloatBits;

u32 fn_80007D58(f32 *arg0) {
    u32 flags[11];
    FloatBits value3;
    FloatBits value7;
    FloatBits value11;
    FloatBits value0;
    FloatBits value1;
    FloatBits value2;
    FloatBits value4;
    FloatBits value5;
    FloatBits value6;
    FloatBits value8;
    FloatBits value9;
    FloatBits value10;

    flags[0] = 1;
    flags[1] = flags[0];
    flags[2] = flags[0];
    flags[3] = flags[0];
    flags[4] = flags[0];
    flags[5] = flags[0];
    flags[6] = flags[0];
    flags[7] = flags[0];
    flags[8] = flags[0];
    flags[9] = flags[0];
    flags[10] = flags[0];
    value0.f = arg0[0];
    if ((value0.u & 0x7F800000) != 0x7F800000) {
        value1.f = arg0[1];
        if ((value1.u & 0x7F800000) != 0x7F800000) {
            flags[10] = 0;
        }
    }
    if ((s32)flags[10] == 0) {
        value2.f = arg0[2];
        if ((value2.u & 0x7F800000) != 0x7F800000) flags[9] = 0;
    }
    if ((s32)flags[9] == 0) {
        value4.f = arg0[4];
        if ((value4.u & 0x7F800000) != 0x7F800000) flags[8] = 0;
    }
    if ((s32)flags[8] == 0) {
        value5.f = arg0[5];
        if ((value5.u & 0x7F800000) != 0x7F800000) flags[7] = 0;
    }
    if ((s32)flags[7] == 0) {
        value6.f = arg0[6];
        if ((value6.u & 0x7F800000) != 0x7F800000) flags[6] = 0;
    }
    if ((s32)flags[6] == 0) {
        value8.f = arg0[8];
        if ((value8.u & 0x7F800000) != 0x7F800000) flags[5] = 0;
    }
    if ((s32)flags[5] == 0) {
        value9.f = arg0[9];
        if ((value9.u & 0x7F800000) != 0x7F800000) flags[4] = 0;
    }
    if ((s32)flags[4] == 0) {
        value10.f = arg0[10];
        if ((value10.u & 0x7F800000) != 0x7F800000) flags[3] = 0;
    }
    if ((s32)flags[3] == 0) {
        value3.f = arg0[3];
        if ((value3.u & 0x7F800000) != 0x7F800000) flags[2] = 0;
    }
    if ((s32)flags[2] == 0) {
        value7.f = arg0[7];
        if ((value7.u & 0x7F800000) != 0x7F800000) flags[1] = 0;
    }
    if ((s32)flags[1] == 0) {
        value11.f = arg0[11];
        if ((value11.u & 0x7F800000) != 0x7F800000) flags[0] = 0;
    }
    return flags[0];
}
