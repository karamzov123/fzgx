#include "types.h"

// fn_1_E5980: Store float value at offset 0x20 in base pointer
void fn_1_E5980(void *base, f32 value) {
    *(f32 *)((char *)base + 0x20) = value;
}
