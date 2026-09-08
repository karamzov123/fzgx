#include "types.h"

typedef struct {
    u32 unk_0;
} Obj_1_bss_F68_Target;

extern Obj_1_bss_F68_Target *lbl_1_bss_F68;

u32 fn_1_660C(void) {
    Obj_1_bss_F68_Target *obj = lbl_1_bss_F68;

    if (obj != 0) {
        return obj->unk_0 & (1u << 31);
    }

    return 0;
}
