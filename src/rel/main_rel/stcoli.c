#include "types.h"

/* fzgx:begin fn_1_14CB4 */
#include "rel/main_rel/stcoli.h"

extern void fn_1_9D77C(u32, u32);
extern void fn_1_46B4(u32, u32, u8 *, s32);
extern u8 lbl_1_data_51C8[40];
extern u32 lbl_801A6410;

void fn_1_14CB4(void) {
    if (lbl_1_bss_3BE0 != 0) {
        if (lbl_1_bss_3BE0->unk_54 != 0) {
            fn_1_9D77C(lbl_1_bss_3BE0->unk_54, lbl_1_bss_3BE0->unk_48);
        }

        fn_1_46B4(lbl_801A6410, lbl_1_bss_3BE4.unk_0, lbl_1_data_51C8, 0x2c9);
        lbl_1_bss_3BE4.unk_0 = 0;
        fn_1_46B4(lbl_801A6410, (u32)lbl_1_bss_3BE0, lbl_1_data_51C8, 0x2cb);
        lbl_1_bss_3BE0 = 0;
    }
}
/* fzgx:end fn_1_14CB4 */

/* fzgx:begin fn_1_14DC4 */
#include "rel/main_rel/stcoli.h"

u32 fn_1_14DC4(void) {
    if (lbl_1_bss_3BE0 != 0) {
        return lbl_1_bss_3BE0->unk_14;
    }
    return 0;
}
/* fzgx:end fn_1_14DC4 */

/* fzgx:begin fn_1_14F04 */
extern u8 *lbl_801A66CC;

u8 *fn_1_14F04(void) {
    return lbl_801A66CC + 0x1b0;
}
/* fzgx:end fn_1_14F04 */

/* fzgx:begin fn_1_181CC */
// fn_1_181CC: main_rel .text:0x000181CC size 0x24
// Wrapper function that passes first three arguments through and sets fourth to 1

extern void fn_1_180F4(int, int, int, int);

void fn_1_181CC(int a, int b, int c) {
    fn_1_180F4(a, b, c, 1);
}
/* fzgx:end fn_1_181CC */

/* fzgx:begin fn_1_181F0 */
extern void fn_1_180F4(int, int, int, int);

void fn_1_181F0(int a, int b, int c) {
    fn_1_180F4(a, b, c, 0);
}
/* fzgx:end fn_1_181F0 */

/* fzgx:begin fn_1_18FFC */
extern void fn_1_18784(int, int, int, int);

void fn_1_18FFC(int a, int b, int c, int d, int e, int f, int g) {
    fn_1_18784(c, d, e, g);
}
/* fzgx:end fn_1_18FFC */

/* fzgx:begin fn_1_216C0 */
extern void fn_1_16BA0(void *, void *, void *, void *, int);

void fn_1_216C0(void *arg0, void *arg1, void *arg2) {
    int local;

    local = 0;
    fn_1_16BA0(arg0, arg1, arg2, &local, 0);
}
/* fzgx:end fn_1_216C0 */

/* fzgx:begin fn_1_2192C */
extern void fn_1_18214(void *, void *, int);

void fn_1_2192C(void *self, void *arg) {
    fn_1_18214(self, arg, 0);
}
/* fzgx:end fn_1_2192C */

/* fzgx:begin fn_1_21BE4 */
extern void fn_1_18214(void *, void *, int);
extern f32 lbl_1_rodata_6D0;
extern void fn_1_21950(void *, f32, f32);

typedef struct Fn_1_21BE4 {
    u32 flags;
    unsigned char pad04[0x38];
    f32 field3c;
    f32 field40;
} Fn_1_21BE4;

void fn_1_21BE4(Fn_1_21BE4 *self, void *arg) {
    f32 value2;
    f32 value1;

    fn_1_18214(self, arg, 0);
    if ((self->flags & 0x800) != 0) {
        value2 = self->field3c;
    } else {
        value2 = lbl_1_rodata_6D0;
    }
    if ((self->flags & 0x400) != 0) {
        value1 = self->field40;
    } else {
        value1 = lbl_1_rodata_6D0;
    }
    fn_1_21950(arg, value1, value2);
}
/* fzgx:end fn_1_21BE4 */

/* fzgx:begin fn_1_21C5C */
extern void fn_1_18214(void *, void *, int);
extern f32 lbl_1_rodata_6D4;
extern void fn_1_21950(void *, f32, f32);

void fn_1_21C5C(void *self, void *arg) {
    fn_1_18214(self, arg, 0);
    fn_1_21950(arg, lbl_1_rodata_6D4, lbl_1_rodata_6D4);
}
/* fzgx:end fn_1_21C5C */

/* fzgx:begin fn_1_21CA0 */
extern f32 lbl_1_rodata_6D0;
extern void fn_1_21950(void *, f32, f32);

typedef struct Fn_1_21CA0 {
    u32 flags;
    unsigned char pad04[0x38];
    f32 field3c;
    f32 field40;
} Fn_1_21CA0;

void fn_1_21CA0(Fn_1_21CA0 *self, void *arg) {
    f32 value2;
    f32 value1;

    if ((self->flags & 0x800) != 0) {
        value2 = self->field3c;
    } else {
        value2 = lbl_1_rodata_6D0;
    }
    if ((self->flags & 0x400) != 0) {
        value1 = self->field40;
    } else {
        value1 = lbl_1_rodata_6D0;
    }
    fn_1_21950(arg, value1, value2);
}
/* fzgx:end fn_1_21CA0 */

/* fzgx:begin fn_1_235C8 */
extern void *memset(void *, int, unsigned int);

typedef struct Fn_1_235C8 {
    unsigned char *field00;
    unsigned char *field04;
    unsigned char data[0xb8];
} Fn_1_235C8;

void fn_1_235C8(Fn_1_235C8 *self) {
    if (self != 0) {
        memset(self, 0, 0xc0);
        self->field00 = &self->data[0];
        self->field04 = &self->data[0x5c];
    }
}
/* fzgx:end fn_1_235C8 */

/* fzgx:begin fn_1_23610 */
extern void *memset(void *, int, unsigned int);

void fn_1_23610(void *self) {
    if (self != 0) {
        memset(self, 0, 0x194);
    }
}
/* fzgx:end fn_1_23610 */

/* fzgx:begin fn_1_23640 */
extern void *memset(void *, int, unsigned int);

typedef struct Fn_1_23640 {
    unsigned char pad00[0x80];
    int field80;
    unsigned char pad84[0x0c];
    int *field90;
    int *field94;
    unsigned char pad98[0x04];
    int field9c;
    unsigned char padA0[0x18];
    int fieldb8;
    unsigned char padbc[0x0c];
    int fieldc8;
    unsigned char padcc[0x0c];
    int fieldd8;
    unsigned char pade0[0x40];
    int field11c;
    unsigned char pad120[0x3c];
    int field15c;
    unsigned char tail160[0x9c];
} Fn_1_23640;

void fn_1_23640(Fn_1_23640 *self, int value) {
    if (self != 0) {
        if (value != 0) {
            memset(self, 0, 0x1fc);
            self->field11c = -1;
            self->field15c = -1;
        } else {
            memset(self, 0, 0x117);
        }
        self->field80 = value;
        self->field90 = &self->fieldb8;
        self->field94 = &self->fieldc8;
        self->field9c = -1;
        self->fieldb8 = -1;
        self->fieldd8 = -1;
    }
}
/* fzgx:end fn_1_23640 */

/* fzgx:begin fn_1_248C0 */
#include "rel/main_rel/stcoli.h"

extern char lbl_1_bss_9C;
extern void fn_1_A2D84(int value);

typedef struct Fn_1_248C0 {
    u32 unk_00;
    unsigned char pad04[0x471];
    signed char unk_475;
    unsigned char pad476[0x3d];
    unsigned char unk_4b3;
} Fn_1_248C0;

// Updates the collision state and reports whether the current flags trigger a response.
int fn_1_248C0(Fn_1_248C0 *self, int arg) {
    int result;
    int test;

    result = 0;
    if (self->unk_00 & 0x800) {
        if (self->unk_00 & 1) {
            result = 1;
        } else if (self->unk_00 & 0x10000) {
            result = 1;
        }
        if (!(self->unk_00 & 0x10000) && self->unk_4b3 == 0) {
            if (*(s16 *)&lbl_1_bss_960 == 9 && lbl_1_bss_9C == 5) {
            } else if (lbl_1_bss_9C == 6) {
            } else if (self->unk_475 != -1 && !(self->unk_00 & 0x04000200)) {
                fn_1_A2D84(0xA9072100);
            }
            self->unk_4b3 = 1;
        }
    } else {
        test = self->unk_00 & 0x80;
        if (test && self->unk_4b3 == 0 && !(self->unk_00 & 0x10000)) {
            if (self->unk_475 != -1 && !(self->unk_00 & 0x04000200)) {
                fn_1_A2D84(0xA9072200);
            }
            self->unk_4b3 = 1;
        }
        if ((self->unk_00 & 1) && test) {
            if ((self->unk_00 & 0x02810000) || arg) {
                result = 1;
            }
        }
    }
    if ((self->unk_00 & 0x40000000) && (self->unk_00 & 1)) {
        result = 1;
    }
    if (self->unk_00 & 0x10000000) {
        result = 0;
    }
    return result;
}
/* fzgx:end fn_1_248C0 */

/* fzgx:begin fn_1_25870 */
extern unsigned char lbl_1_rodata_840[];

typedef struct Fn_1_25870 {
    u32 flags;
    unsigned char pad04[0x1f4];
    f32 field1f8;
    f32 field1fc;
    unsigned char pad200[0x24];
    f32 field224;
    unsigned char pad228[0x1c];
    u32 field244;
    unsigned char pad248[0x58];
    u32 field2a0;
    unsigned char pad2a4[0x58];
    u32 field2fc;
    unsigned char pad300[0x58];
    u32 field358;
    unsigned char pad35c[0x11a];
    u8 field476;
    u8 field477;
} Fn_1_25870;

void fn_1_25870(Fn_1_25870 *self) {
    unsigned char *data = lbl_1_rodata_840;
    u32 flags;
    int trigger;

    if (self->field476 != 0) {
        self->field476--;
    }
    flags = self->flags;
    if ((flags & 0x2) == 0) {
        if (self->field224 > *(f32 *)(data + 0x18)) {
            if ((flags & 0x2000) != 0) {
                self->field244 &= ~4;
                self->field2a0 &= ~4;
                self->field2fc &= ~4;
                self->field358 &= ~4;
                self->field476 = self->field477;
            } else {
                trigger = 0;
                if ((flags & 0x1000) != 0 &&
                    __fabs(self->field1fc) > *(f64 *)(data + 0x1b8)) {
                    trigger = 1;
                }
                if ((self->flags & 0x8) != 0) {
                    trigger = 1;
                }
                if (trigger != 0) {
                    self->field244 |= 4;
                    self->field2a0 |= 4;
                    self->field2fc |= 4;
                    self->field358 |= 4;
                }
            }
        }
    } else {
        self->field244 &= ~4;
        self->field2a0 &= ~4;
        self->field2fc &= ~4;
        self->field358 &= ~4;
    }
    flags = self->flags;
    if ((flags & 0x4000) != 0 &&
        __fabs(self->field1fc) < *(f64 *)(data + 0x1b8)) {
        self->flags = flags & ~0x4000;
    }
    if (__fabs(self->field1f8) > *(f64 *)(data + 0x1c0)) {
        self->flags |= 0x4000;
    }
    if ((self->flags & 0x4000) == 0) {
        return;
    }
    self->field244 |= 0x10;
    self->field2a0 |= 0x10;
    self->field2fc |= 0x10;
    self->field358 |= 0x10;
}
/* fzgx:end fn_1_25870 */

/* fzgx:begin fn_1_28660 */
extern void lbl_8006DAEC(void);
extern void lbl_8006DB30(void);
extern int fn_1_8C66C(short value);
extern int fn_1_163BC(void *arg0, void *arg1, int mask, int value);

typedef struct Fn_1_28660 {
    int field00;
    short field04;
    unsigned char pad06[0x212];
    int field218;
    unsigned char pad21c[0x2e0];
    int field4fc;
    unsigned char pad500[0x8c];
    int field58c;
} Fn_1_28660;

void fn_1_28660(Fn_1_28660 *self) {
    int result;
    int mask;

    lbl_8006DAEC();

    mask = 0x88;
    if ((self->field58c & 0x2) == 0) {
        mask |= 0x10;
    }
    if ((self->field4fc & 0x00100000) == 0) {
        mask |= 0x2;
    }
    if ((self->field4fc & 0x00080000) == 0) {
        mask |= 0x20;
    }
    if ((self->field4fc & 0x00040000) == 0) {
        mask |= 0x40;
    }
    if ((self->field4fc & 0x00020000) == 0) {
        mask |= 0x80;
    }

    if ((self->field00 & 0x2) != 0) {
        result = 0;
    } else {
        result = fn_1_163BC((char *)self + 0x7c, (char *)self + 0x88,
            mask, fn_1_8C66C(self->field04));
    }

    if ((result & 0x8) != 0) {
        self->field00 |= 0x00600000;
        self->field218 |= 0x10000000;
    }
    if ((self->field4fc & 0x00100000) != 0 ||
        (result & 0x2) != 0) {
        if ((self->field00 & 0x80) == 0) {
            self->field58c |= 1;
            self->field218 |= 0x08000000;
        }
    }
    if ((self->field00 & 0x20) == 0) {
        if ((self->field4fc & 0x00040000) != 0 ||
            (result & 0x40) != 0) {
            self->field218 |= 0x20000000;
        }
    }
    if ((self->field4fc & 0x00080000) != 0 ||
        (result & 0x20) != 0) {
        self->field218 |= 0x80000000u;
    }
    if ((result & 0x10) != 0) {
        self->field218 |= 0x04000000;
    }
    if ((self->field4fc & 0x00020000) != 0 ||
        (result & 0x80) != 0) {
        self->field218 |= 0x02000000;
    }

    lbl_8006DB30();
}
/* fzgx:end fn_1_28660 */

/* fzgx:begin fn_1_2A3E4 */
typedef struct Fn_1_2A3E4 {
    u32 flags;
    unsigned char pad04[0x186];
    u8 active;
    unsigned char pad18b[0x85];
    u16 value210;
} Fn_1_2A3E4;

void fn_1_2A3E4(Fn_1_2A3E4 *self) {
    if (self->active != 0) {
        if (self->flags & 0x00200000) {
            self->value210 += 0x14;
        } else {
            self->value210 += 0xA;
        }
    }
}
/* fzgx:end fn_1_2A3E4 */

/* fzgx:begin fn_1_2A638 */
extern void lbl_8006E1D8(void *arg0, f32 arg1, f32 arg2, f32 arg3);

typedef struct Fn_1_2A638 {
    unsigned char pad00[0x08];
    f32 field08;
    f32 field0c;
    f32 field10;
    unsigned char pad14[0x30];
    f32 field44;
    f32 field48;
} Fn_1_2A638;

void fn_1_2A638(void *arg0, Fn_1_2A638 *self) {
    lbl_8006E1D8((char *)self + 0x14, self->field08,
        self->field0c + self->field44 - self->field48, self->field10);
}
/* fzgx:end fn_1_2A638 */

/* fzgx:begin fn_1_2A678 */
typedef struct Fn_1_2A678_Values {
    int x;
    int y;
    int z;
} Fn_1_2A678_Values;

typedef struct Fn_1_2A678_Source {
    unsigned char pad00[0x7c];
    Fn_1_2A678_Values values;
} Fn_1_2A678_Source;

typedef struct Fn_1_2A678_Dest {
    unsigned char pad00[0xc];
    Fn_1_2A678_Values values;
} Fn_1_2A678_Dest;

void fn_1_2A678(Fn_1_2A678_Source *self, Fn_1_2A678_Dest *dest) {
    dest->values = self->values;
}
/* fzgx:end fn_1_2A678 */

/* fzgx:begin fn_1_2C7C4 */
void fn_1_2C7C4(u8 *value, const u8 *target, int step) {
    if (value[0] != target[0]) {
        if (value[0] > target[0]) {
            if (value[0] > target[0] + step) {
                value[0] -= step;
            } else {
                value[0] = target[0];
            }
        } else {
            if (value[0] < target[0] - step) {
                value[0] += step;
            } else {
                value[0] = target[0];
            }
        }
    }
    if (value[1] != target[1]) {
        if (value[1] > target[1]) {
            if (value[1] > target[1] + step) {
                value[1] -= step;
            } else {
                value[1] = target[1];
            }
        } else {
            if (value[1] < target[1] - step) {
                value[1] += step;
            } else {
                value[1] = target[1];
            }
        }
    }
    if (value[2] != target[2]) {
        if (value[2] > target[2]) {
            if (value[2] > target[2] + step) {
                value[2] -= step;
            } else {
                value[2] = target[2];
            }
        } else {
            if (value[2] < target[2] - step) {
                value[2] += step;
            } else {
                value[2] = target[2];
            }
        }
    }
    if (value[3] != target[3]) {
        if (value[3] > target[3]) {
            if (value[3] > target[3] + step) {
                value[3] -= step;
            } else {
                value[3] = target[3];
            }
        } else {
            if (value[3] < target[3] - step) {
                value[3] += step;
            } else {
                value[3] = target[3];
            }
        }
    }
}
/* fzgx:end fn_1_2C7C4 */

/* fzgx:begin fn_1_3FCD4 */
extern u32 lbl_1_data_5B80;

void fn_1_3FCD4(u32 mask) {
    if (mask == 0) {
        lbl_1_data_5B80 = 0xffffffff;
    } else {
        lbl_1_data_5B80 &= ~mask;
    }
}
/* fzgx:end fn_1_3FCD4 */
