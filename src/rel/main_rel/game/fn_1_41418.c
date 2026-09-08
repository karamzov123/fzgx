#include "types.h"

typedef struct Fn41418Data {
    u32 count;
    char *strings;
} Fn41418Data;

extern u32 strlen(const char *str);

static inline u32 add_lengths(u32 length, u32 address) {
    return length + address;
}

char *fn_1_41418(Fn41418Data *data, u32 index) {
    u32 count = data->count & 0x7fffffff;
    u32 str = (u32)data->strings;

    if (index >= count) {
        return 0;
    }

    while (index != 0) {
        str = add_lengths(strlen((char *)str), str);
        index -= 1;
        str += 1;
    }

    return (char *)str;
}
