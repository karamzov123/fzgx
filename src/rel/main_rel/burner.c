#include "types.h"

/* fzgx:begin fn_1_98590 */
typedef struct Fn198590Node Fn198590Node;

struct Fn198590Node {
    u8 unk_00[0x04];
    Fn198590Node *next;
    u8 unk_08[0x6C];
    f32 value_74;
    u8 unk_78[0x42C];
    f32 value_4A4;
};

typedef struct {
    u8 unk_00[0x04];
    Fn198590Node *head;
} Fn198590Root;

extern Fn198590Root *lbl_1_bss_6EA04;
extern f32 lbl_1_rodata_4104[59];
extern s32 lbl_1_bss_6EA08;

void fn_1_98590(void) {
    Fn198590Node *node = lbl_1_bss_6EA04->head;
    f32 value = lbl_1_rodata_4104[0];

    while (node != 0) {
        Fn198590Node *next = node->next;
        node->value_74 = value;
        node->value_4A4 = value;
        node = next;
    }

    lbl_1_bss_6EA08 += 1;
    if (lbl_1_bss_6EA08 >= 4) {
        lbl_1_bss_6EA08 = 0;
    }
}
/* fzgx:end fn_1_98590 */

/* fzgx:begin fn_1_985EC */
typedef struct Fn1985Node Fn1985Node;

struct Fn1985Node {
    u8 unk_00[0x04];
    Fn1985Node *next;
    u8 unk_08[0x40];
    u32 key;
    u8 unk_4C[0x28];
    f32 value;
    u8 unk_78[0x430];
    u32 flags;
};

typedef struct {
    u8 unk_00[0x04];
    Fn1985Node *head;
} Fn1985Root;

extern Fn1985Root *lbl_1_bss_6EA04;
extern f32 lbl_1_rodata_4104[59];

void fn_1_985EC(u32 key) {
    Fn1985Node *node = lbl_1_bss_6EA04->head;
    f32 value = lbl_1_rodata_4104[0];
    u32 zero = 0;

    while (node != 0) {
        Fn1985Node *next = node->next;
        if (node->key == key) {
            node->value = value;
            node->flags = zero;
        }
        node = next;
    }
}
/* fzgx:end fn_1_985EC */

/* fzgx:begin fn_1_988D8 */
// fn_1_988D8: empty in retail (single blr).
void fn_1_988D8(void) {
}
/* fzgx:end fn_1_988D8 */

/* fzgx:begin fn_1_9A0A4 */
// fn_1_9A0A4: empty in retail (single blr).
void fn_1_9A0A4(void) {
}
/* fzgx:end fn_1_9A0A4 */

/* fzgx:begin fn_1_9A578 */
extern s16 lbl_1_data_2A7E0;
extern void fn_1_10302C(void);

typedef void (*fn_1_9A578_callback)(void);

struct fn_1_9A578_entry {
    u8 pad[8];
    fn_1_9A578_callback callback;
    u8 rest[0x18];
};

extern struct fn_1_9A578_entry lbl_1_data_2ABAC[];

void fn_1_9A578(void) {
    if (lbl_1_data_2A7E0 > 0) {
        fn_1_10302C();
        lbl_1_data_2ABAC[lbl_1_data_2A7E0].callback();
    }
}
/* fzgx:end fn_1_9A578 */

/* fzgx:begin fn_1_9A770 */
typedef void (*fn_1_9A770_callback)(void);

struct fn_1_9A770_data {
    u8 pad[0x38];
    fn_1_9A770_callback callback;
};

extern struct fn_1_9A770_data lbl_1_data_2A7E0;

void fn_1_9A770(void) {
    if (lbl_1_data_2A7E0.callback != 0) {
        lbl_1_data_2A7E0.callback();
    }
}
/* fzgx:end fn_1_9A770 */

/* fzgx:begin fn_1_9CC6C */
typedef struct {
    u8 pad[0x30];
    u32 value30;
    u32 value34;
} GlobalState;

extern void fn_1_9DB04(void);
extern void fn_1_9C724(void);
extern void fn_1_9BBE4(void *, s32, s32, s32);
extern u32 lbl_1_data_2A7E0[62];
extern GlobalState *lbl_801A66CC;

void fn_1_9CC6C(void *arg0, s32 arg1) {
    if (arg0 != 0) {
        fn_1_9DB04();
        fn_1_9C724();
        fn_1_9BBE4(arg0, 0, arg1 - 1, 0);
        lbl_801A66CC->value30 = lbl_1_data_2A7E0[26];
        lbl_801A66CC->value34 = lbl_1_data_2A7E0[27];
    }
}
/* fzgx:end fn_1_9CC6C */

/* fzgx:begin fn_1_9CCE8 */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern void lbl_8006D7B0(s32);
extern void fn_8006E294(Vec3 *);
extern s32 lbl_8006D24C(f32, f32);
extern void fn_8006E324(s16);
extern void fn_8006E398(s32);

void fn_1_9CCE8(s32 arg0) {
    Vec3 value;
    s32 result;

    switch (arg0) {
    case 1:
        lbl_8006D7B0(arg0);
        break;
    case 2:
        fn_8006E294(&value);
        result = lbl_8006D24C(value.z, value.y);
        fn_8006E324((s16)(result - 0x4000));
        break;
    case 3:
        fn_8006E294(&value);
        result = lbl_8006D24C(value.x, value.z);
        fn_8006E398(result);
        break;
    default:
        break;
    }
}
/* fzgx:end fn_1_9CCE8 */

/* fzgx:begin fn_1_9D230 */
extern u32 fn_1_9D260(void);

u32 fn_1_9D230(void) {
    return (fn_1_9D260() & 0xC) != 0;
}
/* fzgx:end fn_1_9D230 */
