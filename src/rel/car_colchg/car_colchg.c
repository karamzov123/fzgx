#include "types.h"
#include "rel/car_colchg/globals.h"

extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
void fn_1_426C(s16);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
s32 fn_1_4C10(void);
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
void fn_1_A8EB0(s32, s32);
s32 fn_1_7F49C(s16, s32, void *);
void fn_1_A8DD4(const char *, ...);
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;
void fn_1_80058(s8, struct CarColchgEntry *);
void fn_1_80C18(u32, struct CarColchgEntry *, s32);
void fn_1_426C(s16);
s32 fn_1_4C10(void);
void fn_1_A8EB0(s32, s32);
s32 fn_1_7F49C(s16, s32, void *);
void fn_1_A8DD4(const char *, ...);
void fn_1_80058(s8, struct CarColchgEntry *);
void fn_1_80C18(u32, struct CarColchgEntry *, s32);
extern struct CarColchgEntry lbl_9_data_1B8[];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;
extern struct Struct_lbl_1_bss_970 lbl_1_bss_970;
extern u16 lbl_1_bss_96A;
extern u32 lbl_9_bss_0;
extern struct Entry *lbl_1_data_1FB6C[];

/* fzgx:begin colchg_menu_init */
extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;
extern struct CarColchgEntry lbl_9_data_1B8[];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;

extern u32 lbl_9_bss_10;
extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), s32, s32);
void fn_1_4A00(s32, s32, u32);

void colchg_menu_init(void) {
    // Register the callback, then apply the initial collision-color state.
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_2C, colchg_menu_disp, 0, 8);
    fn_1_4A00(1, 15, lbl_9_bss_10);
}
/* fzgx:end colchg_menu_init */

/* fzgx:begin fn_9_454 */
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef int BOOL;
typedef unsigned long size_t;
struct Struct_lbl_1_bss_D58
{
  u8 pad0[8];
  volatile u16 unk8;  // volatile: decomp-permuter found retail keeps this access order; a revise pass should express it without volatile
  u8 padA[6];
  volatile u16 unk10;  // volatile: decomp-permuter found retail keeps this access order; a revise pass should express it without volatile
  volatile u16 unk12;  // volatile: decomp-permuter found retail keeps this access order; a revise pass should express it without volatile
};
struct Struct_lbl_1_bss_970
{
  u8 pad0[4];
  u8 unk4;
};
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
void fn_9_454(void)
{
  s32 value;
  s8 delta;
  if (fn_1_4C10() == 0)
  {
    delta = 0;
    if (((lbl_1_bss_D58.unk10 >> 3) & 1) || ((long) ((lbl_1_bss_D58.unk12 >> 3) & 1)))
    {
      delta = -1;
    }
    if (((lbl_1_bss_D58.unk10 >> 2) & 1) || ((lbl_1_bss_D58.unk12 >> 2) & 1))
    {
      delta++;
    }
    if (1)
    {
      value = lbl_1_bss_970.unk4 + delta;
      value = (lbl_1_bss_970.unk4 = (value > 0x73) ? (0x73) : ((value < 0x73) ? (0x73) : (value)));
      if ((lbl_1_bss_D58.unk8 >> 8) & 1)
      {
        lbl_1_bss_96A = value;
      }
      if ((lbl_1_bss_D58.unk8 >> 9) & 1)
      {
        lbl_1_bss_96A = 0x75;
      }
    }
  }
}
/* fzgx:end fn_9_454 */

/* fzgx:begin fn_9_534 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;
void fn_1_435C(u32);

// Initialize the car color-change state and apply the current selection.
void fn_9_534(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_534 */

/* fzgx:begin fn_9_6F0 */
extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;
extern struct CarColchgEntry lbl_9_data_1B8[];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;

extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;

extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);

// Apply the current car color-change configuration.
void fn_9_6F0(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_6F0 */

/* fzgx:begin fn_9_A18 */
struct Entry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
};
void fn_1_435C(u32);

struct CarColchgState {
    u8 pad0[8];
    s16 *selection;
    u32 padC;
    u32 value10;
    u8 pad14[0xC8];
    s16 valueDC;
    u32 valueE0;
};

void fn_9_A18(void) {
    struct CarColchgState *state = (struct CarColchgState *)&lbl_9_bss_0;
    struct Entry *entry;

    entry = lbl_1_data_1FB6C[*state->selection];
    fn_1_435C(state->value10);
    fn_1_426C(state->valueDC);
    entry[state->selection[2]].unk8 = state->valueE0;
}
/* fzgx:end fn_9_A18 */

/* fzgx:begin fn_9_DC0 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);

// Refresh the collision-change state from the current global settings.
void fn_9_DC0(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_DC0 */

/* fzgx:begin colchg_ezsel_init */
extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;
extern struct CarColchgEntry lbl_9_data_1B8[];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;

extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;

extern u32 lbl_9_bss_10;
extern u16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), int, int);

// Initialize the collision-color table and retain its generated handle.
void colchg_ezsel_init(void) {
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_9C, colchg_ezsel_disp, 0, 8);
}
/* fzgx:end colchg_ezsel_init */

/* fzgx:begin fn_9_F38 */
extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);

// Initialize the color-change subsystem and apply its current configuration.
void fn_9_F38(void) {
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_F38 */

/* fzgx:begin colchg_save_init */
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern u32 lbl_9_bss_10;
extern u16 lbl_9_bss_DC;

void fn_1_435C(u32);
u16 fn_1_3F8C(char *, void (*)(void), u32, u32);

// Initialize the color-change system and select its default update interval.
void colchg_save_init(void) {
    fn_1_435C(lbl_9_bss_10);
    lbl_9_bss_DC = fn_1_3F8C(lbl_9_data_B0, colchg_save_disp, 0, 8);
    lbl_1_bss_968 = 10;
}
/* fzgx:end colchg_save_init */

/* fzgx:begin fn_9_FD0 */
struct Struct_lbl_1_bss_D58 {
    u8 pad[8];
    u16 unk8;
};

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

// Initializes the color-change subsystem from its configured state.
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

/* fzgx:begin fn_9_10BC */
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef int BOOL;
typedef unsigned long size_t;
struct Struct_lbl_1_bss_D58
{
  u8 pad[8];
  u16 unk8;
};
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
inline u16 *inline_fn(u16 *arg0)
{
  return arg0;
}

void fn_9_10BC(void)
{
  u16 value;
  u16 *new_var;
  u16 *p;
  if (fn_1_4C10() == 0)
  {
    p = &lbl_1_bss_96A;
    value = 0x73;
    *p = value;
    new_var = inline_fn(p);
    if ((lbl_1_bss_D58.unk8 >> 9) & 1)
    {
      *new_var = 0x72;
    }
    if (((lbl_1_bss_D58.unk8 >> 8) & 1) != 0U)
    {
      lbl_1_bss_96A = 0x76;
    }
  }
}
/* fzgx:end fn_9_10BC */

/* fzgx:begin fn_9_1124 */
extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;
extern struct CarColchgEntry lbl_9_data_1B8[];
extern s16 lbl_1_bss_962;
extern u32 lbl_1_bss_71688;
extern u32 lbl_1_bss_7168C;

extern char lbl_9_data_2C[17];
extern void colchg_menu_disp(void);
extern char lbl_9_data_9C[18];
extern void colchg_ezsel_disp(void);
extern char lbl_9_data_B0[17];
extern void colchg_save_disp(void);
extern u16 lbl_1_bss_968;
extern struct Struct_lbl_1_bss_D58 lbl_1_bss_D58;
extern u32 lbl_9_bss_8;
extern char lbl_9_data_18C[16];
extern char lbl_9_data_19C[7];
extern char lbl_9_data_1A4[18];
extern u32 lbl_9_bss_4;

extern u32 lbl_9_bss_10;
extern s16 lbl_9_bss_DC;

void fn_1_435C(u32);

void fn_9_1124(void) {
    // Refresh the active color-change state and apply the current selection.
    fn_1_435C(lbl_9_bss_10);
    fn_1_426C(lbl_9_bss_DC);
}
/* fzgx:end fn_9_1124 */

/* fzgx:begin colchg_ezsel_disp */
void colchg_ezsel_disp(void) {
    u8 buffer[0x20];
    s32 value;

    fn_1_A8EB0(0x14, 0x14);
    value = fn_1_7F49C(*(s16 *)lbl_9_bss_8, 0, buffer);
    fn_1_A8DD4(lbl_9_data_18C, *(s16 *)lbl_9_bss_8, 0x29, value);
}
/* fzgx:end colchg_ezsel_disp */

/* fzgx:begin colchg_save_disp */
void colchg_save_disp(void) {
    fn_1_A8EB0(0x14, 0x14);
    fn_1_A8DD4(lbl_9_data_19C);
}
/* fzgx:end colchg_save_disp */

/* fzgx:begin fn_9_1AC4 */
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

/* fzgx:begin fn_9_1C54 */
struct CarColchgEntry {
    u8 pad[0x20];
    void (*func)(void);
    u32 value24;
    u32 value28;
};

void fn_9_1C54(void) {
    struct CarColchgEntry *entry;
    s16 index;

    entry = lbl_9_data_1B8;
    index = lbl_1_bss_962;
    entry += index - 0x71;

    lbl_1_bss_71688 = entry->value24;
    lbl_1_bss_7168C = entry->value28;
    entry->func();
}
/* fzgx:end fn_9_1C54 */
