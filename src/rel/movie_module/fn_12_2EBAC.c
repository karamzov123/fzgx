#include "types.h"

typedef struct MovieObject MovieObject;
typedef s32 (*MovieCallback)(MovieObject *, s32 *, s32 *);

struct MovieObject {
    u8 pad_000[0x44];
    s32 field_044;
    s32 field_048;
    s32 field_04c;
    s32 field_050;
    u8 pad_054[0x910];
    s32 field_964;
    u8 pad_968[0x358];
    MovieCallback callbacks[16];
    u8 pad_d00[0x208];
    s32 field_f08;
    s32 field_f0c;
    u8 pad_f10[0x14];
    s32 field_f24;
    s32 field_f28;
    u8 pad_f2c[0x1c];
    s32 field_f48;
    s32 field_f4c;
    u8 pad_f50[0x8];
    s32 field_f58;
};

extern u32 lbl_12_bss_7C64[137];
extern s32 fn_12_2D73C(MovieObject *obj, s32 id);
extern void fn_12_24970(s32 *);
extern void fn_12_24950(s32 *);
extern s32 fn_12_2E6AC(MovieObject *obj, s32 *, s32 *);

void fn_12_2EBAC(void) {
    MovieObject **table = (MovieObject **)&lbl_12_bss_7C64[129];
    s32 i;
    s32 flag;
    MovieObject *obj;
    s32 w2;
    s32 w1;
    s32 w0;
    s32 index;
    struct { MovieCallback value; } callback;

    lbl_12_bss_7C64[108]++;

    for (i = 0; i < 8; i++) {
        obj = table[i];
        if (obj != 0) {
            if (obj->field_048 != 4) {
                flag = 0;
            } else if (obj->field_050 != 0) {
                flag = 0;
            } else if (obj->field_964 != 0) {
                flag = 0;
            } else {
                flag = 1;
            }
            if (flag) {
                obj->field_f24 = obj->field_f24 + obj->field_f28;
            }
            if (obj->field_f48 == -1) {
                flag = 0;
            } else if (obj->field_04c != 4) {
                flag = 0;
            } else {
                flag = 1;
            }
            if (flag) {
                obj->field_f48 = obj->field_f48 + obj->field_f28;
            }
            if (fn_12_2D73C(obj, 0x47) == 1) {
                fn_12_24970(&w0);
                index = fn_12_2D73C(obj, 15);
                callback.value = obj->callbacks[index];
                if (callback.value == 0) {
                    callback.value = fn_12_2E6AC;
                }
                callback.value(obj, &w1, &w2);
                fn_12_24950(&w0);
                if (obj->field_f08 != w1 || obj->field_f0c != w2) {
                    if (fn_12_2D73C(obj, 0x47) == 1) {
                        obj->field_f4c = obj->field_f24;
                    } else {
                        obj->field_f4c = obj->field_f58;
                    }
                    obj->field_f08 = w1;
                    obj->field_f0c = w2;
                }
                obj->field_044 = 1;
            }
        }
    }
}
