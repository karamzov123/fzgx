#include "types.h"

struct TRKInitializeSerialHandler_lbl_801A5098 {
    u32 unk_0;
    u8 pad_4[0x4];
    u32 unk_8;
    u32 unk_C;
};

extern struct TRKInitializeSerialHandler_lbl_801A5098 lbl_801A5098[];
static const char fzgx_pool_strings_lbl_800956C0_0[36] = "TRK_Packet_Header \t    %ld bytes\n";
static const char fzgx_pool_strings_lbl_800956C0_24[36] = "TRK_CMD_ReadMemory     %ld bytes\n";
static const char fzgx_pool_strings_lbl_800956C0_48[36] = "TRK_CMD_WriteMemory    %ld bytes\n";
static const char fzgx_pool_strings_lbl_800956C0_6C[32] = "TRK_CMD_Connect \t    %ld bytes\n";
static const char fzgx_pool_strings_lbl_800956C0_8C[32] = "TRK_CMD_ReplyAck\t    %ld bytes\n";
static const char fzgx_pool_strings_lbl_800956C0_AC[36] = "TRK_CMD_ReadRegisters\t%ld bytes\n";

extern void MWTRACE(u32, ...);

s32 TRKInitializeSerialHandler(void) {
    
    lbl_801A5098[0].unk_0 = -1;
    lbl_801A5098[0].unk_8 = 0;
    lbl_801A5098[0].unk_C = 0;
    
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_0)), 64);
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_24)), 64);
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_48)), 64);
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_6C)), 64);
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_8C)), 64);
    MWTRACE(1, (u32)(((u8 *)fzgx_pool_strings_lbl_800956C0_AC)), 64);
    return 0;
}
