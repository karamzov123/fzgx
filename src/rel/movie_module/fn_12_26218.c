#include "types.h"
typedef struct Sig_fn_12_654C_MovieValues {
    s32 value_00; s32 value_04; s32 value_08; s32 value_0c;
    s32 value_10; s32 value_14; s32 value_18; s32 value_1c;
} Sig_fn_12_654C_MovieValues;
typedef struct Sig_fn_12_654C_MovieModule {
    u8 pad00[0x10]; int field10; u8 field14[0x0c];
    Sig_fn_12_654C_MovieValues values;
    Sig_fn_12_654C_MovieValues slots[3];
    u8 fielda0[0x20];
} Sig_fn_12_654C_MovieModule;
typedef void (*Sig_fn_12_24A88_MovieCallback)(void *, s32);
typedef struct Sig_fn_12_24A88_MovieObject {
    u8 pad_0000[0x48]; s32 state; u8 pad_004c[0x940];
    Sig_fn_12_24A88_MovieCallback callback;
    void *callback_context; s32 callback_data;
} Sig_fn_12_24A88_MovieObject;
typedef struct Sig_fn_12_68D4_MovieValues {
    u32 value_00; u32 value_04; u32 value_08; u32 value_0c;
    u32 value_10; u32 value_14; u32 value_18; u32 value_1c;
} Sig_fn_12_68D4_MovieValues;
typedef struct Sig_fn_12_68D4_MovieModule {
    u8 _pad00[0x20]; Sig_fn_12_68D4_MovieValues values;
} Sig_fn_12_68D4_MovieModule;
struct fn_12_26218_Arg0 { u8 pad_0[0x1AEC]; u32 unk_1AEC; };
struct fn_12_26218_Arg3 { u32 unk_0; };
extern int fn_12_654C(Sig_fn_12_654C_MovieModule *, const u8 *, int, int *, int *);
extern int fn_12_68D4(Sig_fn_12_68D4_MovieModule *, Sig_fn_12_68D4_MovieValues *);
extern s32 fn_12_231F8(u32);
extern s32 fn_12_24A88(Sig_fn_12_24A88_MovieObject *, s32);
extern s32 fn_12_23250(u32);
extern s32 fn_12_2559C(struct fn_12_26218_Arg0 *, const u8 *, s32, u32);
extern u32 fn_12_25FA8(struct fn_12_26218_Arg0 *, const u8 *, s32, void *, void *);
extern u32 fn_12_67A4(const u8 *);
extern void *fn_12_57F0(void *, const void *, u32);
u32 fn_12_26218(struct fn_12_26218_Arg0 *arg0, const u8 *arg1, u32 arg2, struct fn_12_26218_Arg3 *arg3, u32 arg4) {
    s32 arg2_local = arg2;
    u32 base;
    s32 result = 0;
    u32 entry;
    u32 data;
    s32 amount;
    s32 count;
    s32 zero;
    s32 i;
    const s8 *p;
    u8 *slot;
    s32 tail;
    Sig_fn_12_68D4_MovieValues loc_18;
    u32 loc_14;
    u32 loc_10;
    u32 loc_C;
    u32 loc_8;
    arg3->unk_0 = 0;
    base = *(u32 *)arg0->unk_1AEC;
    if (fn_12_2559C(arg0, arg1, arg2_local, arg4) == 0) return 0;
    if (fn_12_654C((Sig_fn_12_654C_MovieModule *)base, arg1, arg2_local, (int *)&loc_10, (int *)&loc_14) != 0)
        result = fn_12_24A88((Sig_fn_12_24A88_MovieObject *)arg0, 0xff000d03);
    if (loc_14 & 0x20000) {
        entry = *(u32 *)((u8 *)arg0 + 0x2908);
        if (entry == 0) entry = 0;
        else if (*(s32 *)(arg0->unk_1AEC + 0x14) > 0) entry = 0;
        else entry += 0x8a0;
        if (entry != 0 && *(s32 *)entry == 0) {
            data = entry + 0x24;
            fn_12_68D4((Sig_fn_12_68D4_MovieModule *)base, &loc_18);
            amount = 0xb0;
            if (arg2_local < 0xb0) amount = arg2_local;
            if ((s32)loc_18.value_0c > 0) {
                *(s32 *)(data + 0x160) = amount;
            } else if ((s32)loc_18.value_08 > 0) {
                *(s32 *)(data + 0x164) = amount;
                data += 0xb0;
            } else goto copy_done; /* Keep the verified branch to copy_done. */
            fn_12_57F0((void *)data, arg1, amount);
        }
    }
copy_done:
    if (loc_14 == 0x80000 && fn_12_23250((u32)arg0) != 0) {
        (*(s32 *)(arg0->unk_1AEC + 0x14))++;
        arg3->unk_0 = 4;
    } else if (loc_14 == 0x80000 && fn_12_231F8((u32)arg0) != 0) {
        arg3->unk_0 = 4;
    } else if ((s32)loc_14 == 0) {
        count = *(s32 *)((u8 *)arg0 + 0x28);
        if (arg2_local >= count + 3) {
            p = (const s8 *)arg1;
            for (i = 0; i < count; i++) {
                if (*p++ != 0) { zero = 0; goto zero_done; /* Keep the verified branch to zero_done. */ }
            }
            zero = 1;
zero_done:
            if (zero) { tail = count; goto scan_done; /* Keep the verified branch to scan_done. */ }
        }
        count = 0;
        while (arg2_local >= 4) {
            if (fn_12_67A4(arg1) & 0xd0000) { tail = count; goto scan_done; /* Keep the verified branch to scan_done. */ }
            count++; arg1++; arg2_local--;
        }
        if (arg2_local > 0 && arg2_local < 4) {
            slot = (u8 *)arg0 + *(s32 *)((u8 *)arg0 + 0x1af4) * 0x74;
            if (*(s32 *)(slot + 0x1150) == 0 &&
                (*(s32 *)(slot + 0x1160) != 0 || *(s32 *)(slot + 0x1164) != 0)) {
                zero = 0;
            } else if (arg1 + arg2_local == (const u8 *)(*(u32 *)(slot + 0x1158) + *(u32 *)(slot + 0x115c))) {
                zero = 1;
            } else zero = 0;
            if (zero) count += arg2_local;
        }
        tail = count;
scan_done:
        arg3->unk_0 = tail;
    } else if (!(loc_14 & 0x40000)) {
        result = fn_12_24A88((Sig_fn_12_24A88_MovieObject *)arg0, 0xff000d05);
    } else {
        arg1 += loc_10; arg2_local -= loc_10;
        result = fn_12_25FA8(arg0, arg1, arg2_local, &loc_C, &loc_8);
        if ((s32)loc_8 == 1) arg3->unk_0 = loc_10 + loc_C;
    }
    return result;
}
