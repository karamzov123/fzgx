#include "types.h"
typedef u32 (*fn_12_38340_Fn0)(u32, u32);
typedef u32 (*fn_12_38340_Fn1)(u32, u32);
struct fn_12_38340_Arg0 {
 u8 pad_0[0x158];
 u32 arena;
 u32 capacity;
 u32 current;
 u32 used;
 s32 count;
 u32 unk_16C[32];
};
struct fn_12_38340_Arg1 {
 u8 pad_0[8];
 s32 unk_8;
 s32 unk_C;
};
extern u32 *fn_12_38DBC(void);
extern char lbl_12_rodata_1DF4[33];
extern void MWSFSVM_Error(const char *, ...);
static inline u32 allocate(struct fn_12_38340_Arg0 *arg0, s32 size) {
 u32 result;
 u32 *manager;
 if (arg0->count >= 32) {
  MWSFSVM_Error(lbl_12_rodata_1DF4);
  return 0;
 }
 if (size < 0) return 0;
 if (arg0->arena != 0) {
  if (arg0->used + size > arg0->capacity) result = 0;
  else {
   result = arg0->current;
   arg0->current = result + size;
   arg0->used = arg0->used + size;
  }
 } else {
  manager = fn_12_38DBC();
  result = ((fn_12_38340_Fn0)manager[10])(manager[12], size);
 }
 if (result != 0) {
  arg0->unk_16C[arg0->count] = result;
  arg0->count++;
 }
 return result;
}
s32 fn_12_38340(struct fn_12_38340_Arg0 *arg0, struct fn_12_38340_Arg1 *arg1, u32 *arg2) {
 s32 v0;
 s32 v1;
 s32 v2;
 s32 v3;
 s32 v5;
 s32 v6;
 s32 v7;
 s32 v13 = 0;
 v0 = (arg1->unk_8 + 15) / 16;
 v1 = (arg1->unk_C + 15) / 16;
 v2 = v0 << 4;
 v3 = v1 << 4;
 v5 = v3 / 2;
 v6 = ((v2 + 31) / 32) << 5;
 v7 = v3 * v6 + ((v5 * (((v2 / 2 + 31) / 32) << 5)) << 1) + 32;
 arg2[0] = allocate(arg0, v7);
 arg2[1] = allocate(arg0, v7);
 if (arg2[0] == 0 || arg2[1] == 0) v13 = -1;
 return v13;
}
