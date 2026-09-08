#include "types.h"

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
