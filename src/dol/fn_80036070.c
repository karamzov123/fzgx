#include "types.h"

typedef struct Fn80036070Data {
    u32 flags;
    u8 pad_04[4];
    u32 dimensions;
    u32 scaled_value;
    u8 pad_10[4];
    u32 value_14;
    u8 pad_18[7];
    u8 enabled;
} Fn80036070Data;

void fn_80036070(const Fn80036070Data *data, u32 *out_scaled, u16 *out_width, u16 *out_height,
                 u32 *out_value, u32 *out_flags_low, u32 *out_flags_high, u8 *out_enabled) {
    *out_scaled = (data->scaled_value & 0x001FFFFFu) << 5;
    *out_width = (u16)((data->dimensions & 0x3FFu) + 1);
    *out_height = (u16)(((data->dimensions >> 10) & 0x3FFu) + 1);
    *out_value = data->value_14;
    *out_flags_low = data->flags & 3u;
    *out_flags_high = (data->flags >> 2) & 3u;
    *out_enabled = (u8)(!((data->enabled & 1u) - 1u));
}
