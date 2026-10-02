#include "types.h"

extern u32 lbl_801A6744;
extern u32 lbl_801A6740;
extern u32 lbl_801A673C;
extern u32 lbl_801A6738;
extern u32 lbl_801A6410;
extern void fn_80009468(void *);

typedef struct {
    s32 field_0x0;
    s32 field_0x4;
    s32 field_0x8;
} HeapDesc;

u32 fn_80008EC8(void *arenaStart, void *arenaEnd, s32 maxHeaps) {
    u32 arraySize;
    s32 i;
    HeapDesc *hd;
    u32 zero = 0;
    u32 end;
    u32 ret;

    arraySize = maxHeaps * 12;
    lbl_801A6744 = (u32)arenaStart;
    lbl_801A6740 = maxHeaps;

    for (i = 0; i < (s32)lbl_801A6740; i++) {
        hd = (HeapDesc *)((u8 *)lbl_801A6744 + i * 12);
        hd->field_0x0 = -1;
        hd->field_0x4 = hd->field_0x8 = 0;
    }

    end = lbl_801A6744 + arraySize;
    lbl_801A6738 = (u32)arenaEnd & 0xFFFFFFE0;
    lbl_801A6410 = -1;
    ret = (end + 0x1F) & 0xFFFFFFE0;
    lbl_801A673C = ret;
    fn_80009468((void *)end);
    return ret;
}
