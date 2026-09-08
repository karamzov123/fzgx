#include "types.h"

typedef struct Fn800454DCContext {
    u8 pad[0x30];
    u32 value;
} Fn800454DCContext;

u32 fn_800454DC(Fn800454DCContext *context) {
    return context->value;
}
