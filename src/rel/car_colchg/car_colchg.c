#include "types.h"

/* fzgx:begin fn_9_3F0 */
extern u32 lbl_9_bss_10;
extern char lbl_9_data_2C[17];
extern void fn_9_1160(void);
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), s32, s32);
void fn_1_4A00(s32, s32, u32);

void fn_9_3F0(void) {
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_2C, fn_9_1160, 0, 8);
    fn_1_4A00(1, 15, lbl_9_bss_10);
}
/* fzgx:end fn_9_3F0 */

/* fzgx:begin fn_9_534 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_534(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_534 */

/* fzgx:begin fn_9_6F0 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_6F0(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_6F0 */

/* fzgx:begin fn_9_DC0 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_DC0(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_DC0 */

/* fzgx:begin fn_9_DFC */
extern u32 lbl_9_bss_10;
extern char lbl_9_data_9C[18];
extern void fn_9_1A20(void);
extern u16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), int, int);

void fn_9_DFC(void) {
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_9C, fn_9_1A20, 0, 8);
}
/* fzgx:end fn_9_DFC */

/* fzgx:begin fn_9_F38 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_F38(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_F38 */

/* fzgx:begin fn_9_F74 */
extern u32 lbl_9_bss_10;
extern char lbl_9_data_B0[17];
extern void fn_9_1A8C(void);
extern u16 lbl_9_bss_DC;
extern u16 lbl_1_bss_968;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), u32, u32);

void fn_9_F74(void) {
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_B0, fn_9_1A8C, 0, 8);
    lbl_1_bss_968 = 10;
}
/* fzgx:end fn_9_F74 */

/* fzgx:begin fn_9_FD0 */
struct Struct_lbl_1_bss_D58 {
    u8 pad[8];
    u16 unk8;
};

extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u16 lbl_1_bss_968;

s32 fn_1_4C10(void);

void fn_9_FD0(void) {
    if (fn_1_4C10() == 0) {
        if ((lbl_1_bss_D58.unk8 >> 9) & 1) {
            lbl_1_bss_968 = 0xA;
        }
    }
}
/* fzgx:end fn_9_FD0 */

/* fzgx:begin fn_9_1018 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_1018(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_1018 */

/* fzgx:begin fn_9_1054 */
extern void fn_9_1BCC(void);
extern u32 lbl_9_bss_10;
extern u8 lbl_9_data_C4[140];
extern void fn_9_1AC4(void);
extern u16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(u8 *, void (*)(void), u32, u32);
void fn_1_4A00(u32, u32, u32);

void fn_9_1054(void) {
    u32 value;

    fn_9_1BCC();
    fn_1_435C(lbl_9_bss_10);
    value = fn_1_3F8C(lbl_9_data_C4, fn_9_1AC4, 0, 8);
    lbl_9_bss_DC = value;
    fn_1_4A00(1, 15, lbl_9_bss_10);
}
/* fzgx:end fn_9_1054 */

/* fzgx:begin fn_9_1124 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
void fn_1_426C(s16);

void fn_9_1124(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_1124 */

/* fzgx:begin fn_9_1A20 */
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];

void fn_1_A8EB0(s32, s32);
s32 fn_1_7F49C(s16, s32, void *);
void fn_1_A8DD4(char *, s16, s32, ...);

void fn_9_1A20(void) {
    u8 buffer[0x20];
    s32 value;

    fn_1_A8EB0(0x14, 0x14);
    value = fn_1_7F49C(*(s16 *)lbl_9_bss_8, 0, buffer);
    fn_1_A8DD4(lbl_9_data_18C, *(s16 *)lbl_9_bss_8, 0x29, value);
}
/* fzgx:end fn_9_1A20 */

/* fzgx:begin fn_9_1A8C */
extern char lbl_9_data_19C[7];

void fn_1_A8EB0(s32, s32);
void fn_1_A8DD4(const char *, ...);

void fn_9_1A8C(void) {
    fn_1_A8EB0(0x14, 0x14);
    fn_1_A8DD4(lbl_9_data_19C);
}
/* fzgx:end fn_9_1A8C */

/* fzgx:begin fn_9_1AC4 */
extern char lbl_9_data_1A4[18];

void fn_1_A8EB0(s32, s32);
void fn_1_A8DD4(const char *, ...);

void fn_9_1AC4(void) {
    fn_1_A8EB0(0x14, 0xa);
    fn_1_A8DD4(lbl_9_data_1A4);
}
/* fzgx:end fn_9_1AC4 */

/* fzgx:begin fn_9_1BCC */
struct CarColchgEntry {
    s16 unk0;
    u16 unk2;
    u32 unk4;
    u8 pad8[24];
};

extern u32 lbl_9_bss_4;

void fn_1_80058(s8, struct CarColchgEntry *);
void fn_1_80C18(u32, struct CarColchgEntry *, s32);

void fn_9_1BCC(void) {
    struct CarColchgEntry entries[0x29];
    s8 i;

    for (i = 0; i < 0x29; i++) {
        fn_1_80058(i, &entries[i]);
        entries[i].unk4 = 1;
        entries[i].unk0 = i;
    }
    fn_1_80C18(lbl_9_bss_4, entries, 0x29);
}
/* fzgx:end fn_9_1BCC */
