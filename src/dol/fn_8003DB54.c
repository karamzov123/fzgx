#include "types.h"

typedef struct Color {
    u8 r, g, b, a;
} Color;

typedef struct Entry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    Color unkC;
} Entry;

extern const Color lbl_801A71B8;
extern Entry *lbl_801A6C48;

void fn_8003DB54(u8 arg0, u32 arg1, u32 arg2) {
    Color c = lbl_801A71B8;

    lbl_801A6C48[arg0].unk0 = arg1;
    lbl_801A6C48[arg0].unk4 = arg2;
    lbl_801A6C48[arg0].unk8 = -1;
    lbl_801A6C48[arg0].unkC = c;
}
