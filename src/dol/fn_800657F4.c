#include "types.h"
#include "dol/globals.h"

extern void fn_80064FDC(u8 arg0);

struct Fields {
    u8 pad[0x48c];
    u8 *entry;
    u8 a, b, c, d;
};

void fn_800657F4(u8 arg0) {
    u32 offset = arg0 * 16;
    u8 *resource = *(u8 **)((u8 *)lbl_801A6C80 + offset + 8);
    u8 *data = resource + *(u32 *)(resource + 0x18);
    u16 entry;
    *(u8 *)((u8 *)lbl_801A6C80 + 0x490 + offset) = 0xff;
    *(u8 *)((u8 *)offset + (lbl_801A6C80 + 0x491)) = 0x7f;
    entry = *(u16 *)(resource + 0x3e);
    if (entry != 0) {
        *(u8 **)((u8 *)offset + (lbl_801A6C80 + 0x48c)) = data + entry;
        if ((*(u8 **)((u8 *)lbl_801A6C80 + offset + 0x48c))[1] != 0x80) {
            *(u8 *)((u8 *)lbl_801A6C80 + offset + 0x493) = (*(u8 **)((u8 *)lbl_801A6C80 + offset + 0x48c))[1];
        }
        fn_80064FDC(arg0);
    } else {
        *(u8 **)((u8 *)offset + (lbl_801A6C80 + 0x48c)) = 0;
    }
}
