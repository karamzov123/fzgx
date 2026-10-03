#include "types.h"

typedef struct {
    u32 flag;
    u32 packed;
    u32 value;
} movie_bitstream_out;

#define READ_BITS(n, result) \
    if (bits >= 32 - (n)) { \
        bits -= 32 - (n); \
        if (bits) { \
            word |= next >> ((n) - bits); \
            result = word >> (32 - (n)); \
            word = next << bits; \
        } else { \
            result = word >> (32 - (n)); \
            word = next; \
        } \
        next = *p++; \
    } else { \
        result = word >> (32 - (n)); \
        word <<= (n); \
        bits += (n); \
    }
#define SKIP_BITS(n) \
    bits += (n); \
    if (bits >= 32) { \
        bits -= 32; \
        word = next << bits; \
        next = *p++; \
    } else { \
        word <<= (n); \
    }

void fn_12_62F8(movie_bitstream_out *out, const u8 *data, u32 *kind)
{
    const u8 *start = data + 4;
    const u32 *p = (const u32 *)((u32)start & ~3);
    int bits = ((u32)start - (u32)p) * 8;
    u32 word = *p++;
    u32 next = *p++;
    u32 a, c, e, g, j;
    word <<= bits;
    if (bits >= 30) {
        bits -= 30;
        if (bits) {
            word |= next >> 1;
            a = word >> 30;
            word = next << 1;
        } else {
            a = word >> 30;
            word = next;
        }
        next = *p++;
    } else {
        a = word >> 30;
        word <<= 2;
        bits += 2;
    }
    SKIP_BITS(2);
    READ_BITS(3, c);
    SKIP_BITS(1);
    READ_BITS(15, e);
    SKIP_BITS(1);
    READ_BITS(15, g);
    SKIP_BITS(1);
    SKIP_BITS(1);
    READ_BITS(22, j);
    out->flag = (a == 0) ? 1 : 0;
    out->packed = (c << 28) | (e << 13) | (g >> 2);
    out->value = j;
    *kind = 12;
}
