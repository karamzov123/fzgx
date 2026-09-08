#include "types.h"

/* fzgx:begin fn_1_FB9DC */
extern u8 lbl_1_bss_84454[3124];

typedef struct {
    u8 pad14[0x14];
    int field14;
    u8 pad18[0x5c];
    int field74;
    u8 pad78[0x5c];
    int fieldd4;
    u8 padd8[0x5c];
    int field134;
    u8 pad138[0x5c];
    int field194;
    u8 pad198[0x5c];
    int field1f4;
    u8 pad1f8[0x78];
} State;

void fn_1_FB9DC(int index) {
    State *state = (State *)lbl_1_bss_84454 + (index & 0xff);

    state->field14 = 0;
    state->field134 = 0;
    state->field74 = 0;
    state->field194 = 0;
    state->fieldd4 = 0;
    state->field1f4 = 0;

    switch (index & 0xff) {
    case 0:
        state->field14 = 1;
        state->field134 = 1;
        break;
    case 1:
        state->field134 = 1;
        state->field194 = 1;
        break;
    case 2:
        state->field134 = 1;
        state->field1f4 = 1;
        break;
    case 3:
        state->field134 = 1;
        state->field1f4 = 1;
        break;
    case 4:
        state->field134 = 1;
        state->field194 = 1;
        break;
    default:
        break;
    }
}
/* fzgx:end fn_1_FB9DC */

/* fzgx:begin fn_1_FC40C */
// fn_1_FC40C: empty in retail (single blr).
void fn_1_FC40C(void) {
}
/* fzgx:end fn_1_FC40C */

/* fzgx:begin fn_1_FC410 */
// fn_1_FC410: empty in retail (single blr).
void fn_1_FC410(void) {
}
/* fzgx:end fn_1_FC410 */

/* fzgx:begin fn_1_FCF50 */
extern void fn_1_FCA10(void);

int fn_1_FCF50(void) {
    fn_1_FCA10();
    return 1;
}
/* fzgx:end fn_1_FCF50 */

/* fzgx:begin fn_1_FDFF4 */
extern u8 lbl_1_bss_850C6[14];

void fn_1_FDFF4(void) {
    lbl_1_bss_850C6[0] = 1;
}
/* fzgx:end fn_1_FDFF4 */

/* fzgx:begin fn_1_FE5E0 */
// fn_1_FE5E0: No-op return
void fn_1_FE5E0(void) {
    return;
}
/* fzgx:end fn_1_FE5E0 */

/* fzgx:begin fn_1_FE640 */
// fn_1_FE640: empty in retail (single blr).
void fn_1_FE640(void) {
}
/* fzgx:end fn_1_FE640 */

/* fzgx:begin fn_1_FE780 */
// fn_1_FE780: Empty return
void fn_1_FE780(void) {
}
/* fzgx:end fn_1_FE780 */

/* fzgx:begin fn_1_FE784 */
typedef struct {
    u8 pad[0x3c];
    void *object;
} FE784Data;

typedef struct {
    u8 pad[0x10];
    s32 active;
} FE784Object;

extern FE784Data lbl_1_data_2A7E0;
extern void fn_1_9AD88(FE784Data *);
extern void fn_1_10069C(FE784Object *);
extern void fn_1_FF038(FE784Object *);

void fn_1_FE784(void) {
    FE784Object *object;

    object = (FE784Object *)lbl_1_data_2A7E0.object;
    fn_1_9AD88(&lbl_1_data_2A7E0);
    if (object->active != 0) {
        fn_1_10069C(object);
    }
    fn_1_FF038(object);
}
/* fzgx:end fn_1_FE784 */

/* fzgx:begin fn_1_FE7D4 */
// fn_1_FE7D4: empty in retail (single blr).
void fn_1_FE7D4(void) {
}
/* fzgx:end fn_1_FE7D4 */

/* fzgx:begin fn_1_1013C0 */
// fn_1_1013C0: empty in retail (single blr).
void fn_1_1013C0(void) {
}
/* fzgx:end fn_1_1013C0 */

/* fzgx:begin fn_1_101400 */
// fn_1_101400: empty in retail (single blr).
void fn_1_101400(void) {
}
/* fzgx:end fn_1_101400 */

/* fzgx:begin fn_1_101424 */
// fn_1_101424: empty in retail (single blr).
void fn_1_101424(void) {
}
/* fzgx:end fn_1_101424 */

/* fzgx:begin fn_1_101428 */
extern void fn_1_9AD88(void);

void fn_1_101428(void) {
    fn_1_9AD88();
}
/* fzgx:end fn_1_101428 */

/* fzgx:begin fn_1_101448 */
// fn_1_101448: Empty function (blr only)
void fn_1_101448(void) {
}
/* fzgx:end fn_1_101448 */

/* fzgx:begin fn_1_10144C */
// fn_1_10144C: returns a constant.
int fn_1_10144C(void) {
    return 0;
}
/* fzgx:end fn_1_10144C */
