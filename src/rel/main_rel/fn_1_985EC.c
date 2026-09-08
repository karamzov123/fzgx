#include "types.h"

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
