#include "types.h"

typedef struct Fn80028090Node Fn80028090Node;

struct Fn80028090Node {
    Fn80028090Node *next;
    u8 pad4[4];
    void *data;
};

extern u32 OSDisableInterrupts(void);
extern Fn80028090Node *lbl_801A6B90;
extern void fn_80026D70(void *data);
extern void OSRestoreInterrupts(u32 level);

void fn_80028090(void) {
    u32 level;
    Fn80028090Node *node;

    level = OSDisableInterrupts();
    node = lbl_801A6B90;
    while (node != 0) {
        fn_80026D70(node->data);
        node = node->next;
    }
    lbl_801A6B90 = 0;
    OSRestoreInterrupts(level);
}
