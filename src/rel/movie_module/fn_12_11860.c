#include "types.h"

typedef struct MovieModule MovieModule;
typedef void (*MovieCallback)(MovieModule *, s32, void *, void *);

struct MovieModule {
    u8 unk_000[0x234];
    u8 unk_234[0x10];
    u8 unk_244[0x4c];
    MovieCallback callback;
    u8 unk_294[0x74];
    s32 unk_308;
};

extern void fn_12_112D4(void *, void *, void *);

void fn_12_11860(MovieModule *module, s32 count) {
    u32 local;
    void *first = module->unk_234;
    void *second = (u8 *)first + 0x10;

    while (--count) {
        module->callback(module, module->unk_308 - count, &local, first);
        fn_12_112D4(&local, first, second);
    }
}
