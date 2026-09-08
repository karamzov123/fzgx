#include "types.h"

extern u32 fn_800576DC(void);
extern u32 fn_80057728(u32);

void fn_800583DC(u32 arg0) {
    fn_80057728(arg0);
    *(u32 *)((u8 *)arg0 + 12) = 0;
    *(u32 *)((u8 *)arg0 + 16) = *(u32 *)((u8 *)arg0 + 32);
    *(u32 *)((u8 *)arg0 + 20) = 0;
    *(u32 *)((u8 *)arg0 + 24) = 0;
    *(u32 *)((u8 *)arg0 + 40) = 0;
    *(u32 *)((u8 *)arg0 + 44) = 0;
    *(u32 *)((u8 *)arg0 + 48) = 0;
    *(u32 *)((u8 *)arg0 + 52) = 0;
    fn_800576DC();
}
