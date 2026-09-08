#include "types.h"

extern char lbl_12_rodata_38[2];
extern s32 strncmp(void *movie, char *name, s32 value);

s32 fn_12_59C(void *movie, s32 index) {
    if (movie == 0 || index < 0) {
        return 0;
    }
    return strncmp((char *)movie + 0x13, lbl_12_rodata_38, 1) == 0;
}
