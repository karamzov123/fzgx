#include "types.h"

extern u32 fn_8008A754(u32);
extern u32 fn_8008CB30(u32, u32);

s32 fn_8008D8A8(u32 arg0) {
    u32 v0;
    u32 v1;
    u32 t0, t1;
    t0 = fn_8008A754(arg0);
    v0 = t0;
    if ((s32)t0 == 0) {
    v0 = 1;
    } else {
    v0 = 211;
    t1 = fn_8008CB30(v0, arg0);
    v0 = t1;
    v1 = (v0 & 0xFF);
    switch ((s32)v1) {
    case 0:
    v0 = 0;
    break;
    case 2:
    v0 = 2;
    break;
    default:
    v0 = 1;
    }
    }
    return v0;
}
