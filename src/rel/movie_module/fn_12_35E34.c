#include "types.h"

extern void* fn_12_38DBC(void*);

void* fn_12_35E34(void* arg0, void* arg1, void* arg2) {
    void* result;

    result = fn_12_38DBC(arg0);
    *(void**)((u8*)result + 0x28) = arg0;
    *(void**)((u8*)result + 0x2C) = arg1;
    *(void**)((u8*)result + 0x30) = arg2;
    return result;
}
