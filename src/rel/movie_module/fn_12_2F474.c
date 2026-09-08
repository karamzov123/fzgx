#include "types.h"

struct MovieConfig {
    char pad0[4];
    u32 item0;
    u32 item1;
    u32 item2;
    u32 item3;
    u32 item4;
    char pad1[8];
    u32 item5;
};

struct Movie {
    char pad0[0xb44];
    u32 result0;
    u32 result1;
    char pad1[0x640];
    u32 value118c;
    char pad2[0x70];
    u32 value1200;
    u32 value1204;
    char pad3[0x6c];
    u32 value1274;
    u32 value1278;
    char pad4[0x6c];
    u32 value12e8;
    u32 value12ec;
    char pad5[0x6c];
    u32 value135c;
    u32 value1360;
    char pad6[0x6c];
    u32 value13d0;
    u32 value13d4;
    char pad7[0x6c];
    u32 value1444;
    u32 value1448;
    char pad8[0x6c];
    u32 value14b8;
    u32 value14bc;
    char pad9[0x5f4];
    u32 value1ab4;
    char pad10[0x80];
    u32 value1b38;
    u32 value1b3c;
    char pad11[0x3c];
    u32 value1b7c;
    u32 value1b80;
    char pad12[0x3c];
    u32 value1bc0;
    u32 value1bc4;
    char pad13[0x3c];
    u32 value1c04;
    u32 value1c08;
    char pad14[0x3c];
    u32 value1c48;
    char pad15[0x40];
    u32 value1c8c;
    char pad16[0x40];
    u32 value1cd0;
};

extern void fn_12_2F2F4(struct Movie *movie);
extern void fn_12_2D7DC(struct Movie *movie, int a, int b);

int fn_12_2F474(struct Movie *movie, struct MovieConfig *config) {
    if (config->item0 != 0) {
        movie->value1ab4 = 0;
        movie->value118c = 0;
        fn_12_2F2F4(movie);
    } else if (config->item1 != 0) {
        movie->value1ab4 = 1;
        movie->value1200 = 0;
        movie->value1204 = 2;
        movie->value1b38 = 1;
        movie->value1b3c = 3;
        movie->value12e8 = 2;
        if (config->item3 != 0) {
            movie->value12ec = 4;
            movie->value1bc0 = 3;
            movie->value1bc4 = 5;
            movie->value13d0 = 4;
            movie->value13d4 = 6;
            movie->value1c48 = 5;
        } else {
            movie->value12ec = 6;
            movie->value1c48 = 3;
        }
        fn_12_2D7DC(movie, 6, 0);
        movie->result1 = 0;
    } else if (config->item2 != 0) {
        movie->value1ab4 = 2;
        movie->value1274 = 0;
        movie->value1278 = 3;
        movie->value1b7c = 2;
        movie->value1b80 = 4;
        movie->value135c = 3;
        if (config->item4 != 0) {
            movie->value1360 = 5;
            movie->value1c04 = 4;
            movie->value1c08 = 6;
            movie->value1444 = 5;
            movie->value1448 = 7;
            movie->value1c8c = 6;
        } else {
            movie->value1360 = 7;
            movie->value1c8c = 4;
        }
        fn_12_2D7DC(movie, 5, 0);
        movie->result0 = 0;
    } else if (config->item5 != 0) {
        movie->value1ab4 = 7;
        movie->value14b8 = 0;
        movie->value14bc = 8;
        movie->value1cd0 = 7;
        fn_12_2D7DC(movie, 6, 0);
        fn_12_2D7DC(movie, 5, 0);
        movie->result1 = 0;
        movie->result0 = 0;
    } else {
        return -1;
    }
    return 0;
}
