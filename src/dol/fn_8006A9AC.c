#include "types.h"

typedef struct Fn8006A9ACData {
    u8 _pad[8];
    u32 value;
} Fn8006A9ACData;

u32 fn_8006A9AC(Fn8006A9ACData *data) {
    return data->value;
}
