#include "types.h"

extern void fn_12_2D7DC(void *, int, int);

struct MovieItem {
    char pad0[8];
    void *field8;
    void *fieldC;
    void *field10;
    void *field14;
    char pad1[8];
    void *field20;
};

struct Movie {
    char pad0[0xB44];
    int fieldB44;
    int fieldB48;
    char pad1[0x644];
    int field1190;
    char pad2[0x6C];
    int field1200;
    int field1204;
    char pad3[0x6C];
    int field1274;
    int field1278;
    char pad4[0x6C];
    int field12E8;
    int field12EC;
    char pad5[0x6C];
    int field135C;
    int field1360;
    char pad6[0x6C];
    int field13D0;
    int field13D4;
    char pad7[0x6C];
    int field1444;
    int field1448;
    char pad8[0x6C];
    int field14B8;
    int field14BC;
    char pad9[0x634];
    int field1AF4;
    int field1AF8;
    int field1AFC;
    int field1B00;
    char pad10[0x34];
    int field1B38;
    int field1B3C;
    char pad11[0x3C];
    int field1B7C;
    int field1B80;
    char pad12[0x3C];
    int field1BC0;
    int field1BC4;
    char pad13[0x3C];
    int field1C04;
    int field1C08;
    char pad14[0x3C];
    int field1C48;
    char pad15[0x40];
    int field1C8C;
    char pad16[0x40];
    int field1CD0;
};

void fn_12_2F2F4(struct Movie *movie, struct MovieItem *item) {
    movie->field1190 = 1;
    movie->field1AF4 = 0;
    if (item->field8 != 0) {
        movie->field1AF8 = 1;
        movie->field1200 = 1;
        movie->field1204 = 2;
        movie->field1B38 = 1;
        movie->field1B3C = 3;
        movie->field12E8 = 2;
        if (item->field10 != 0) {
            movie->field12EC = 4;
            movie->field1BC0 = 3;
            movie->field1BC4 = 5;
            movie->field13D0 = 4;
            movie->field13D4 = 6;
            movie->field1C48 = 5;
        } else {
            movie->field12EC = 6;
            movie->field1C48 = 3;
        }
    } else {
        fn_12_2D7DC(movie, 5, 0);
        movie->fieldB44 = 0;
    }
    if (item->fieldC != 0) {
        movie->field1AFC = 2;
        movie->field1274 = 1;
        movie->field1278 = 3;
        movie->field1B7C = 2;
        movie->field1B80 = 4;
        movie->field135C = 3;
        if (item->field14 != 0) {
            movie->field1360 = 5;
            movie->field1C04 = 4;
            movie->field1C08 = 6;
            movie->field1444 = 5;
            movie->field1448 = 7;
            movie->field1C8C = 6;
        } else {
            movie->field1360 = 7;
            movie->field1C8C = 4;
        }
    } else {
        fn_12_2D7DC(movie, 6, 0);
        movie->fieldB48 = 0;
    }
    if (item->field20 != 0) {
        movie->field1B00 = 7;
        movie->field14B8 = 1;
        movie->field14BC = 8;
        movie->field1CD0 = 7;
    }
}
