#include "types.h"

extern void fn_12_24970(void *);
extern int fn_12_24A88(void *, int);
extern void fn_12_2AB00(void *);
extern void fn_12_24950(void *);

int fn_12_2692C(void *arg0, void *arg1) {
    u8 local[8];
    int index;
    void *context;
    void *entry;
    int result;

    fn_12_24970(local);
    index = *(int *)((u8 *)arg0 + 0x1b3c);
    context = *(void **)((u8 *)arg0 + 0x1b30);
    entry = (u8 *)arg0 + index * 0x74;
    entry = *(void **)((u8 *)entry + 0x1160);
    if (*(int *)entry != 1) {
        result = fn_12_24A88(arg0, 0xff000f0e);
    } else if ((u8 *)entry + 4 != (u8 *)arg1) {
        result = fn_12_24A88(arg0, 0xff000f0f);
    } else {
        *(int *)entry = 0;
        fn_12_2AB00(*(void **)((u8 *)context + 0x70));
        result = 0;
    }
    fn_12_24950(local);
    return result;
}
