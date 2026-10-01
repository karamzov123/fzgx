#include "types.h"

// Donor seed for fn_80053DB4 (addr 0x80053DB4)
// Extracted from adxt_80053A30.c (original name: fn_80053DB4)

void fn_80053DB4(void* obj, void* arg1, int ch, void* arg3)
{
    int* p = (int*)((char*)obj + (ch << 2));
    char* buf_ch = (char*)obj + (ch << 12);
    int r0;
    void* r28;
    void* r26;

    p[1] = (p[1] - 0x40) & 0x3FF;
    r0 = p[1];
    r28 = *(void**)((char*)obj + 0xC);
    r26 = (char*)buf_ch + 0x14 + (r0 << 2);

    *(int*)lbl_801873C0 = adxtNullCallback();

    if (((unsigned int)arg1 & 0x1F) != 0 || ((unsigned int)r28 & 0x1F) != 0) {
        while (1) {
        }
    }

    fn_80051F38(arg1, r28, r26);
    *(int*)(lbl_801873C0 + 0x10) = adxtNullCallback();

    p = (int*)p[1];
    *(int*)lbl_801873C0 = adxtNullCallback();
    ((void (**)(void*, void*, void*))jumptable_80130BC0)[(int)p >> 6](
        (char*)(buf_ch + 0x14) + ((int)p << 2), *(void**)((char*)obj + 0x10), arg3);
    *(int*)(lbl_801873C0 + 0x10) = adxtNullCallback();
}
