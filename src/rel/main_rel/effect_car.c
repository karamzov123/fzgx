#include "types.h"

/* fzgx:begin fn_1_68248 */
#include "rel/main_rel/globals.h"

extern char lbl_1_data_1D8A8[13];
extern void *lbl_801A6410;
extern void fn_1_46B4(void *, void *, char *, int);

typedef struct Fn1_68248Arg {
    char unk_00[0x34];
    void *unk_34;
} Fn1_68248Arg;

// Register the effect resource with the shared effect manager.
void fn_1_68248(Fn1_68248Arg *arg) {
    fn_1_46B4(lbl_801A6410, arg->unk_34, lbl_1_data_1D8A8, 0xe2);
}
/* fzgx:end fn_1_68248 */

/* fzgx:begin fn_1_68B68 */
// Stub function that immediately returns.
void fn_1_68B68(void) {
}
/* fzgx:end fn_1_68B68 */

/* fzgx:begin fn_1_69BBC */
typedef struct EffectCar {
    u8 pad_00[0x1a];
    u16 field_1a;
} EffectCar;

void fn_1_69BBC(EffectCar *car) {
    car->field_1a = 0xffff;
}
/* fzgx:end fn_1_69BBC */

/* fzgx:begin fn_1_6F8D0 */
extern void fn_1_6F90C(void *arg0, void *arg1);

void fn_1_6F8D0(void *arg0, void *arg1) {
    struct EffectData {
        u8 pad_000[0x58c];
        u32 field_58c;
    };
    struct EffectCar {
        u8 pad_00[0x38];
        struct EffectData *field_38;
        u8 pad_3c[0x76];
        u16 field_b2;
    };
    struct EffectCar *self = (struct EffectCar *)arg0;

    if ((self->field_38->field_58c & 0x10) != 0 && self->field_b2 == 0) {
        fn_1_6F90C(arg0, arg1);
    }
}
/* fzgx:end fn_1_6F8D0 */

/* fzgx:begin fn_1_71CA0 */
extern s16 fn_1_72318(void);
extern void fn_1_71D0C(void *arg0, void *arg1);

void fn_1_71CA0(void *arg0, void *arg1, u16 arg2) {
    if (arg2 == 0xb4) {
        ((struct {
            u8 pad[0x58];
            s16 field_58;
        } *)arg0)->field_58 = fn_1_72318();
    }
    if (arg2 == 0xaa) {
        fn_1_71D0C(arg0, arg1);
    }
}
/* fzgx:end fn_1_71CA0 */
