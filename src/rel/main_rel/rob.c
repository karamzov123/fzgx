#include "types.h"

/* fzgx:begin fn_1_961F0 */
typedef struct Fn1961F0Owner Fn1961F0Owner;

struct Fn1961F0Owner {
    u8 unk_00[0x0C];
    void *resource;
};

extern void fn_1_12ABB4(void *arg0);

void fn_1_961F0(Fn1961F0Owner *owner) {
    if (owner->resource != 0) {
        fn_1_12ABB4(owner->resource);
        owner->resource = 0;
    }
}
/* fzgx:end fn_1_961F0 */

/* fzgx:begin fn_1_97174 */
typedef struct Fn197174Owner Fn197174Owner;
typedef struct Fn197174Root Fn197174Root;
typedef struct Fn197174Entry Fn197174Entry;

struct Fn197174Owner {
    u8 unk_00[0x14];
    u8 *base;
};

struct Fn197174Root {
    u8 unk_00[0x08];
    u16 count;
};

struct Fn197174Entry {
    u8 unk_00[0x1C];
    u32 offset;
};

extern Fn197174Root *fn_1_41BDC(void);
extern Fn197174Entry *fn_1_41B18(Fn197174Owner *owner, void *arg1, s32 index);
extern s32 fn_80083BCC(void *arg0, void *arg1);

s32 fn_1_97174(Fn197174Owner *owner, void *arg1, void *arg2) {
    Fn197174Root *root = fn_1_41BDC();
    s32 index = 0;

    while (index < (s32)root->count) {
        Fn197174Entry *entry = fn_1_41B18(owner, arg1, index);
        if (fn_80083BCC(owner->base + entry->offset, arg2) == 0) {
            return index;
        }
        index++;
    }
    return -1;
}
/* fzgx:end fn_1_97174 */

/* fzgx:begin fn_1_9724C */
extern s16 fn_1_98104(void);
extern u32 *lbl_1_bss_6EAD0;
extern void fn_8004C698(u32 arg0);

void fn_1_9724C(void) {
    if (fn_1_98104() == 0) {
        fn_8004C698(lbl_1_bss_6EAD0[1]);
    }
}
/* fzgx:end fn_1_9724C */
