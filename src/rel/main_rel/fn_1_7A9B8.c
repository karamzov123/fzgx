#include "types.h"

typedef struct {
    u8 unk00[0x18];
    s32 count;
    void *nodes;
} Manager;

typedef struct {
    u8 unk00[0xf4];
    void *field_f4;
    u8 unk_f8[0x4b0 - 0xf8];
} Node;

typedef struct {
    u8 unk00[0x2c];
    void *field_2c;
    u8 unk30[0x8c - 0x30];
    void *field_8c;
    u8 unk90[0xa0 - 0x90];
} Entry;

extern Manager *lbl_1_bss_3BE0;
extern Entry *lbl_1_bss_3BE4;

extern void lbl_8006DBAC(void *arg0);
extern void lbl_8006E1B0(void *arg0, void *arg1);

void fn_1_7A9B8(void) {
    Entry *table;
    Node *node;
    s32 i;

    i = 0;
    table = lbl_1_bss_3BE4;
    node = (Node *)lbl_1_bss_3BE0->nodes;
    while (i < lbl_1_bss_3BE0->count) {
        if (node->field_f4 != 0) {
            lbl_8006DBAC(&table->field_2c);
            lbl_8006E1B0(node->field_f4, &table->field_8c);
        }
        i++;
        table++;
        node++;
    }
}
