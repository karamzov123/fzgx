#include "types.h"

typedef struct MovieData {
    u8 unk_90[0x90];
    u32 unk_120;
    u32 unk_124;
} MovieData;

void fn_12_CCB0(const MovieData *movie, u32 *first, u32 *second) {
    *first = movie->unk_120;
    *second = movie->unk_124;
}
