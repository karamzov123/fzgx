#include "types.h"

typedef struct MovieBuffer {
    u8 data[0x894];
} MovieBuffer;

typedef struct MovieData {
    u8 pad_00[0x0c];
    MovieBuffer buffer;
} MovieData;

typedef struct MovieModule {
    u8 pad_00[0x78];
    MovieBuffer buffer;
    u8 pad_90c[0x2908 - 0x90c];
    MovieData *movie;
} MovieModule;

extern void fn_12_57F0(void *work, void *arg, int size);
extern void fn_12_24570(void *data);
extern int fn_12_24E94(MovieModule *self);

int fn_12_24670(MovieModule *self, void *arg, int size) {
    int copy_size;
    MovieBuffer *copy;

    if (*(s32 *)self->buffer.data != 0) {
        return 0;
    }

    copy_size = 0x800;
    if (size < 0x800) {
        copy_size = size;
    }
    fn_12_57F0((u8 *)self + 0x10c, arg, copy_size);
    *(u32 *)((u8 *)self + 0x108) = copy_size;
    fn_12_24570(self->buffer.data);
    *(u32 *)((u8 *)self + 0x930) = *(u32 *)((u8 *)self + 0x9c);
    *(u32 *)((u8 *)self + 0x934) = *(u32 *)((u8 *)self + 0xa0);
    *(u32 *)((u8 *)self + 0x938) = *(u32 *)((u8 *)self + 0xa4);

    if (self->movie == 0) {
        copy = 0;
    } else if (fn_12_24E94(self) > 0) {
        copy = 0;
    } else {
        copy = (MovieBuffer *)((u8 *)self->movie + 0x0c);
    }
    if (copy != 0) {
        *copy = self->buffer;
    }
    return 1;
}
