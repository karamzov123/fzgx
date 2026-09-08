#include "types.h"
#include "dol/globals.h"

typedef struct {
    u8 pad_0[0xAC];
    s32 unk_AC;
    s32 unk_B0;
    s32 unk_B4;
    s32 unk_B8;
    u8 pad_BC[0x14];
    u32 unk_D0;
    void (*unk_D4)(s32, s32);
} BlockReadControlBlock;

extern s32 __CARDReadSegment(s32 index, void (*callback)(s32, s32));
extern void __CARDPutControlBlock(BlockReadControlBlock *block, s32 result);

void BlockReadCallback(s32 index, s32 result) {
    BlockReadControlBlock *block;
    void (*callback)(s32, s32);

    block = (BlockReadControlBlock *)((u8 *)&lbl_80177960 + index * 0x110);
    if (result >= 0) {
        block->unk_B8 += 0x200;
        block->unk_B0 += 0x200;
        block->unk_B4 += 0x200;
        if (--block->unk_AC > 0) {
            result = __CARDReadSegment(index, BlockReadCallback);
            if (result >= 0) {
                return;
            }
        }
    }
    if (block->unk_D0 == 0) {
        __CARDPutControlBlock(block, result);
    }
    callback = block->unk_D4;
    if (callback != 0) {
        block->unk_D4 = 0;
        callback(index, result);
    }
}
