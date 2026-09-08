#include "types.h"

typedef struct MovieEntry {
    int field_0;
    char unk_4[0x40];
} MovieEntry;

typedef struct MovieModuleState {
    char unk_0[0x1aac];
    MovieEntry entries[1];
} MovieModuleState;

int fn_12_2F1B4(MovieModuleState *state, int index) {
    return state->entries[index].field_0 != 0;
}
