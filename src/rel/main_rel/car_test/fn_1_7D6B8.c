#include "types.h"
#include "rel/main_rel/globals.h"
#include "rel/main_rel/car_test.h"

extern void *memset(void *dst, int value, u32 size);
extern u8 fn_1_816E8(void *);
extern void fn_800793D4(u32, int, int);

typedef struct {
    u8 pad_18[0x18];
    u16 unk_18;
} Fn1_7D6B8Info;
typedef struct {
    u8 pad_334[0x334];
    Fn1_7D6B8Info **unk_334;
    s32 *unk_338;
} Fn1_7D6B8BlockEntry;
typedef struct { u8 *value; } Fn1_7D6B8ColorContainer;
typedef struct {
    void *base;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    s8 field_7;
    s8 field_8;
    u8 pad_9[3];
    u32 *field_c;
    void *field_10, *field_14;
    u8 *field_18;
    Fn1_7D6B8ColorContainer *unk_1c;
    u32 field_20;
    s8 field_24;
    s8 field_25[5];
} Fn1_7D6B8Arg;
#define INPUT(o) (*(u16 *)((u8 *)&lbl_1_bss_9F8 + (o)))
#define VINPUT(o) (*(volatile u16 *)((u8 *)&lbl_1_bss_9F8 + (o))) /* Retail reloads this memory-mapped input each read. */
#define BIT(o,b) ((INPUT(o) >> (b)) & 1)
#define VBIT(o,b) ((VINPUT(o) >> (b)) & 1)
#define BOTH(b) (VBIT(0x10,b) || BIT(0x12,b))
#define ALL(b) (VBIT(0x10,b) || BIT(0x12,b) || VBIT(0x24,b) || VBIT(0x26,b))
#define COLOR(n) { u8 *p; u8 *q; u8 byte; p = arg->unk_1c->value; var_r0.value = 255; p[n] += var_r5; if ((n)==0) { p = arg->unk_1c->value; byte = p[n]; if (byte <= 255U) var_r0.value = byte; p[n] = var_r0.value; } else { q = arg->unk_1c->value; byte = q[n]; if (byte <= 255U) var_r0.value = byte; q[n] = var_r0.value; } }
#define COLOR18(n) { u8 *p; u8 *q; u8 byte; p = arg->field_18; var_r0.value = 255; p[n] += var_r5; if ((n)==0) { p = arg->field_18; byte = p[n]; if (byte <= 255U) var_r0.value = byte; p[n] = var_r0.value; } else { q = arg->field_18; byte = q[n]; if (byte <= 255U) var_r0.value = byte; q[n] = var_r0.value; } }
#pragma opt_dead_assignments off
#pragma opt_pointer_analysis off
#pragma opt_propagation off
#pragma opt_lifetimes off
void fn_1_7D6B8(void *arg0) {
    s8 * fzgx_live_;
    Fn1_7D6B8BlockEntry *block_entry;
    s8 var_r4 = 0;
    s8 var_r5 = 0;
    s8 var_r6 = 0;
    Fn1_7D6B8Arg *arg = arg0;
    u8 tmp_call8;
    s32 tmp_ra5;
    void * fzgx_live;
    s32 temp_addr;
    s32 *temp_p;
    s32 temp_r0_2;
    s32 temp_r8;
    s32 var_r6_2;
    s8 temp_r10;
    s8 var_r4_2;
    s32 temp_r3_4;
    struct { s32 value; } var_r0;
    void *temp_r3_3;
    s8 temp_r4_2;
    s32 temp_bit;
    u32 *temp_r3_2;
    s32 tmp_call5;
    if (!(((Fn1_7D6B8Arg *)lbl_1_bss_6D7E8)->field_20 & 0x40000000)) {
        switch ((s8)arg->field_6) {
        case 0:
            if (BOTH(3)) var_r4 = -1;
            if (BOTH(2)) var_r4 += 1;
            arg->field_4 += var_r4;
            if ((s8)arg->field_4 < 1) arg->field_4 = 4;
            if ((s8)arg->field_4 > 4) arg->field_4 = 1;
            if (BIT(8,8)) { arg->field_6 = arg->field_4; return; }
            break;
        case 1:
            if (BOTH(0)) var_r4 = -1;
            if (BOTH(1)) var_r4 += 1;
            arg->field_7 += var_r4;
            if (arg->field_7 < 0) arg->field_7 = 40;
            if (arg->field_7 > 40) arg->field_7 = 0;
            temp_addr = (s32)arg->base + (s8)arg->field_7 * 0x440;
            temp_addr += (*(s8 volatile *)&(arg->field_24)) /* Retail reloads this field. */ * 12;
            temp_r0_2 = (s8)*(s32 *)(*(s32 *)(temp_addr + 0x338));
            tmp_call5 = BIT(0x14,11);
            if (tmp_call5) {
                if (VBIT(0x26,3)) var_r6 = -1;
                if (BIT(0x26,2)) var_r6 += 1;
                {
                    u8 car = *(volatile u8 *)&arg->field_7; /* Retail reload. */
                    u8 category;
                    category = *(volatile u8 *)&arg->field_24; /* Retail reload. */
                    fzgx_live = *(void *volatile *)&arg->base; /* Retail reload. */
                    temp_r10 = (s8)category;
                    block_entry = (Fn1_7D6B8BlockEntry *)((u8 *)fzgx_live + (s8)car * 0x440);
                    block_entry = (Fn1_7D6B8BlockEntry *)((u8 *)block_entry + temp_r10 * 12);
                    temp_r8 = (s8)(*block_entry->unk_334)->unk_18;
                    if (var_r6 != 0) {
                        var_r6_2 = arg->field_25[temp_r10] + var_r6;
                        temp_r8 -= 1;
                        if (var_r6_2 > temp_r8) var_r6_2 = 0;
                        else if (var_r6_2 < 0) var_r6_2 = temp_r8;
                        arg->field_25[temp_r10] = var_r6_2;
                    }
                }
            }
            if (BOTH(3)) var_r5 = -1;
            if (BOTH(2)) var_r5 += 1;
            arg->field_8 += var_r5;
            if (arg->field_8 < 0) arg->field_8 = temp_r0_2;
            if (arg->field_8 > temp_r0_2) arg->field_8 = 0;
            if (var_r4 != 0) {
                arg->field_8 = temp_r0_2;
                arg->field_25[0] = 0;
                arg->field_25[1] = 0;
                arg->field_25[2] = 0;
                fzgx_live_ = arg->field_25;
                fzgx_live_[3] = 0;
                arg->field_25[4] = 0;
            }
            tmp_ra5 = arg->field_8;
            if (tmp_ra5 != temp_r0_2 && BIT(8,11)) {
                arg->field_c[arg->field_7] ^= 1 << arg->field_8;
            }
            if (BIT(8,10)) {
                temp_r3_2 = &arg->field_c[arg->field_7];
                if (*temp_r3_2 & 0x80000000) memset(temp_r3_2,0,4);
                else memset(temp_r3_2,-1,4);
            }
            if (BIT(8,8)) arg->field_6 = 2;
            if (BIT(8,9)) { arg->field_6 = 0; arg->field_5 = 0; }
            if (BIT(0x1c,4)) arg->field_20 ^= 0x80000000;
            var_r4_2 = 0;
            if (BIT(0x14,11)) {
                if (BIT(0x26,1)) var_r4_2 = 1;
                else if (BIT(0x26,0)) var_r4_2 = -1;
            } else if (BIT(0x12,6) || BIT(0x12,5)) var_r4_2 = 1;
            else if (BIT(0x12,7) || BIT(0x12,4)) var_r4_2 = -1;
            if (var_r4_2 != 0) {
                do {
                    temp_r3_4 = ((var_r4_2) + ((*(s8 volatile *)&(arg->field_24)) /* Retail reloads this field. */));
                    if (temp_r3_4 > 4) var_r0.value = 0;
                    else {
                        var_r0.value = 4;
                        if (temp_r3_4 >= 0) var_r0.value = temp_r3_4;
                    }
                    arg->field_24 = var_r0.value;
                    temp_r4_2 = *(s8 volatile *)&(arg->field_24); /* Retail reloads this field. */
                    temp_bit = 1 << temp_r4_2;
                    temp_r3_3 = (u8 *)(arg->base) + arg->field_7 * 0x440;
                } while (!(temp_bit & *(u8 *)((u8 *)temp_r3_3 + 908)));
                arg->field_8 = fn_1_816E8(temp_r3_3);
                return;
            }
            break;
        case 2:
            if (BIT(8,9)) arg->field_6 = 1;
            if (VBIT(0x24,3)) var_r4 = -1;
            if (VBIT(0x24,2)) var_r4 += 1;
            if (VBIT(0x24,0)) var_r5 = -1;
            if (VBIT(0x24,1)) var_r5 += 1;
            arg->field_5 += var_r4;
            if ((s8)arg->field_5 < 0) arg->field_5 = 2;
            if ((s8)arg->field_5 > 2) arg->field_5 = 0;
            switch ((s8)arg->field_5) {
            case 0: COLOR(0); break;
            case 1: COLOR(1); break;
            case 2: COLOR(2); break;
            case 3: COLOR(3); break;
            }
            if (BIT(0x1c,4)) { arg->field_20 ^= 0x80000000; return; }
            break;
        case 4:
            if (BOTH(3)) var_r4 = -1;
            if (BOTH(2)) var_r4 += 1;
            if (BOTH(0)) var_r5 = -1;
            if (BOTH(1)) var_r5 += 1;
            arg->field_5 += var_r4;
            if ((s8)arg->field_5 < 0) arg->field_5 = 3;
            if ((s8)arg->field_5 > 3) arg->field_5 = 0;
            switch ((s8)arg->field_5) {
            case 0: COLOR18(0); break;
            case 1: COLOR18(1); break;
            case 2: COLOR18(2); break;
            case 3: COLOR18(3); break;
            }
            if (BIT(8,11)) fn_800793D4((u32)arg->field_18,0,4);
            if (BIT(8,9)) { arg->field_6 = 0; arg->field_5 = 0; }
            break;
        case 3:
            if (BIT(8,9)) { arg->field_6 = 0; arg->field_5 = 0; }
            if (ALL(3)) var_r4 = -1;
            if (ALL(2)) var_r4 += 1;
            if (ALL(0)) var_r5 = -1;
            if (ALL(1)) var_r5 += 1;
            arg->field_5 += var_r4;
            if ((s8)arg->field_5 < 0) arg->field_5 = 2;
            if ((s8)arg->field_5 > 2) arg->field_5 = 0;
            switch ((s8)arg->field_5) {
            case 0: COLOR(0); break;
            case 1: COLOR(1); break;
            case 2: COLOR(2); break;
            case 3: COLOR(3); break;
            }
            break;
        case 5: break;
        }
    }
}
#pragma opt_lifetimes reset
