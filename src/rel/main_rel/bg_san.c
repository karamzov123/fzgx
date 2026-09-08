#include "types.h"
#include "rel/main_rel/globals.h"

/* fzgx:begin fn_1_DA7B8 noprologue */
#include "types.h"
extern u32 lbl_1_data_2A7E0[62];
extern void fn_1_DAAFC(u32 value);

void fn_1_DA7B8(void) {
    fn_1_DAAFC(lbl_1_data_2A7E0[15]);
}
/* fzgx:end fn_1_DA7B8 */

/* fzgx:begin fn_1_DAA58 noprologue */
#include "types.h"
typedef struct {
    u8 pad_0000[0xbd4];
    u32 flag;
    u8 state[1];
} State;

extern State *lbl_1_data_2A7E0[62];
extern void fn_1_FB9C0(u32 value);
extern void fn_1_103F10(void *state);
extern void fn_1_DAB3C(State *state);

void fn_1_DAA58(void) {
    State *state = lbl_1_data_2A7E0[15];

    if (state->flag != 0) {
        fn_1_FB9C0(0);
        fn_1_FB9C0(1);
        fn_1_FB9C0(2);
        fn_1_FB9C0(4);
    }
    fn_1_103F10(&state->state[0]);
    fn_1_DAB3C(state);
}
/* fzgx:end fn_1_DAA58 */

/* fzgx:begin fn_1_DAAF8 */
// fn_1_DAAF8: empty in retail (single blr).
void fn_1_DAAF8(void) {
}
/* fzgx:end fn_1_DAAF8 */

/* fzgx:begin fn_1_DAB1C */
extern void fn_1_58C4(void);

void fn_1_DAB1C(void) {
    fn_1_58C4();
}
/* fzgx:end fn_1_DAB1C */

/* fzgx:begin fn_1_DAB3C */
extern void fn_1_58C4(void);

void fn_1_DAB3C(void) {
    fn_1_58C4();
}
/* fzgx:end fn_1_DAB3C */

/* fzgx:begin fn_1_DAD68 */
// fn_1_DAD68: empty in retail (single blr).
void fn_1_DAD68(void) {
}
/* fzgx:end fn_1_DAD68 */

/* fzgx:begin fn_1_DAEF8 */
// fn_1_DAEF8: empty in retail (single blr).
void fn_1_DAEF8(void) {
}
/* fzgx:end fn_1_DAEF8 */

/* fzgx:begin fn_1_DAEFC */
extern s32 fn_1_5910(void);
extern void *fn_1_9D260(void);
extern void fn_1_DB198(void *arg0, void *arg1);
extern s16 fn_1_9AD88(void);
extern s16 fn_1_7B054(void);
extern s16 fn_1_3F0C8(void);
extern f32 lbl_1_rodata_663C;
extern void fn_1_1067A8(void *arg0, f32 arg1, f32 arg2);

// Initializes the current background object and updates it for the active state.
void fn_1_DAEFC(void) {
    s32 index;
    Obj_1_data_2A7E0_At3C *object;

    index = fn_1_5910();
    object = lbl_1_data_2A7E0.unk_3C;
    fn_1_DB198(object, fn_1_9D260());
    fn_1_9AD88();
    if (fn_1_7B054() == 42) {
        if (fn_1_3F0C8() != 39) {
            fn_1_1067A8((u8 *)object + 0x142a0, ((f32 *)object)[index], lbl_1_rodata_663C);
        }
    }
}
/* fzgx:end fn_1_DAEFC */

/* fzgx:begin fn_1_DAF90 */
// fn_1_DAF90: empty in retail (single blr).
void fn_1_DAF90(void) {
}
/* fzgx:end fn_1_DAF90 */

/* fzgx:begin fn_1_DC1B8 */
// fn_1_DC1B8: returns a constant.
int fn_1_DC1B8(void) {
    return 0;
}
/* fzgx:end fn_1_DC1B8 */

/* fzgx:begin fn_1_DC204 */
// fn_1_DC204: empty in retail (single blr).
void fn_1_DC204(void) {
}
/* fzgx:end fn_1_DC204 */

/* fzgx:begin fn_1_DC264 */
// fn_1_DC264: empty in retail (single blr).
void fn_1_DC264(void) {
}
/* fzgx:end fn_1_DC264 */

/* fzgx:begin fn_1_DC3A0 */
// fn_1_DC3A0: empty in retail (single blr).
void fn_1_DC3A0(void) {
}
/* fzgx:end fn_1_DC3A0 */

/* fzgx:begin fn_1_DC3A4 */
extern void fn_1_103090(void *);

typedef struct Entry {
    u8 unk00[0x68];
    s32 initialized;
    u8 unk6c[0x40];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC3A4(Container *container) {
    s32 count;
    Entry *entry;

    count = container->count;
    entry = container->entries;
    while (count > 0) {
        entry->initialized = 1;
        fn_1_103090(entry);
        count--;
        entry++;
    }
}
/* fzgx:end fn_1_DC3A4 */

/* fzgx:begin fn_1_DC5E8 */
extern void fn_1_103264(void *, void *);

typedef struct Entry {
    u8 unk00[0xac];
} Entry;

typedef struct Container {
    s32 count;
    Entry entries[1];
} Container;

void fn_1_DC5E8(Container *container, void *arg) {
    s32 count;
    Entry *entry;

    entry = container->entries;
    count = container->count;
    while (count > 0) {
        fn_1_103264(entry, arg);
        count--;
        entry++;
    }
}
/* fzgx:end fn_1_DC5E8 */
