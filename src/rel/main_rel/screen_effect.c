#include "types.h"

/* fzgx:begin fn_1_72848 */
extern u8 lbl_1_bss_6C8EC[2144];
extern void fn_1_76650(void *arg);
extern u8 lbl_1_bss_6D14C[92];

void fn_1_72848(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        fn_1_76650(&lbl_1_bss_6C8EC[i * 0x10c]);
    }
    lbl_1_bss_6D14C[0] = 0;
}
/* fzgx:end fn_1_72848 */

/* fzgx:begin fn_1_76DBC */
// fn_1_76DBC: empty in retail (single blr).
void fn_1_76DBC(void) {
}
/* fzgx:end fn_1_76DBC */

/* fzgx:begin fn_1_77200 */
extern void fn_1_77238(void);
extern void fn_1_772E0(void);
extern void fn_1_77384(void *arg);

void fn_1_77200(void *arg) {
    fn_1_77238();
    fn_1_772E0();
    fn_1_77384(arg);
}
/* fzgx:end fn_1_77200 */

/* fzgx:begin fn_1_78950 */
extern u8 lbl_1_bss_6D14C[92];
extern void fn_8006FDEC(void);
extern void fn_8006FEFC(void);
extern void fn_8006FD1C(void);

void fn_1_78950(void) {
    int i;

    lbl_1_bss_6D14C[0] = 1;
    for (i = 0; i < 2; i++) {
        fn_8006FDEC();
        fn_8006FEFC();
        fn_8006FD1C();
    }
}
/* fzgx:end fn_1_78950 */

/* fzgx:begin fn_1_7899C */
// fn_1_7899C: main_rel .text:0x0007899C size 0x10
// Zero the first byte of lbl_1_bss_6D14C

extern u8 lbl_1_bss_6D14C[92];

void fn_1_7899C(void) {
    lbl_1_bss_6D14C[0] = 0;
}
/* fzgx:end fn_1_7899C */

/* fzgx:begin fn_1_79100 */
extern void fn_1_9A864(void);
extern u8 fn_1_7B074(void);
extern u32 lbl_1_bss_6D770;
extern u32 lbl_1_bss_6D774;
extern void fn_8006FDEC(void);
extern void fn_80071718(void *arg);
extern void fn_800711A8(void *arg);
extern void fn_1_14CB4(void);
extern void fn_1_FA84(void);
extern s16 lbl_1_data_1DFA4[6];

void fn_1_79100(void) {
    fn_1_9A864();
    if (!fn_1_7B074()) {
        if (lbl_1_bss_6D770 != 0 || lbl_1_bss_6D774 != 0) {
            fn_8006FDEC();
        }
        if (lbl_1_bss_6D770 != 0) {
            fn_80071718((void *)lbl_1_bss_6D770);
            lbl_1_bss_6D770 = 0;
        }
        if (lbl_1_bss_6D774 != 0) {
            fn_800711A8((void *)lbl_1_bss_6D774);
            lbl_1_bss_6D774 = 0;
        }
    }
    fn_1_14CB4();
    fn_1_FA84();
    lbl_1_data_1DFA4[4] = -1;
}
/* fzgx:end fn_1_79100 */

/* fzgx:begin fn_1_7A9B8 */
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
/* fzgx:end fn_1_7A9B8 */

/* fzgx:begin fn_1_7B054 */
extern s16 lbl_1_data_1DFA4[6];

s16 fn_1_7B054(void) {
    return lbl_1_data_1DFA4[4];
}
/* fzgx:end fn_1_7B054 */
