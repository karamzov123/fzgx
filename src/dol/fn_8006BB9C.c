#include "types.h"

typedef struct Fn8006BB9CData {
    u8 _pad[0xf4];
    void *items[64];
} Fn8006BB9CData;

extern s32 fn_8006C134(void *value);

s32 fn_8006BB9C(Fn8006BB9CData *data, u32 index) {
    u32 result;

    result = -2;
    if (data->items[index & 0x3f] != 0) {
        result = fn_8006C134(data->items[index & 0x3f]);
    }
    return result;
}
