#include "types.h"

typedef struct MovieContext {
    u8 pad_0000[0x1ab4];
    void *movie;
} MovieContext;

extern void fn_12_224FC(void *context, void *movie, void *argument);

void fn_12_24E00(MovieContext *context, void *argument) {
    fn_12_224FC(context, context->movie, argument);
}
