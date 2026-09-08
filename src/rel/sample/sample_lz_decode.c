#include "types.h"

typedef struct {
    void *src;
    void *dst;
    u32 flags;
} LzDecodeArgs;

extern u32 *fn_18_444(LzDecodeArgs *args);

// Wraps the LZ decoder: pack (src, dst) into an argument block and return the
// first word of the decoder's result block.
u32 sample_lz_decode(void *src, void *dst) {
    LzDecodeArgs args;
    args.src = src;
    args.dst = dst;
    args.flags = 0;
    return *fn_18_444(&args);
}
