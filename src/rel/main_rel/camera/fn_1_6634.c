#include "types.h"
#include "rel/main_rel/globals.h"

typedef struct {
    u32 flags;
} CameraState;

extern CameraState *fn_1_D960(void);
extern void fn_1_BD54(void);
extern void fn_1_B870(void);

// Dispatches to the active or inactive camera update routine.
void fn_1_6634(void) {
    CameraState *state = fn_1_D960();

    if (((state->flags >> 30) & 1) != 0) {
        fn_1_BD54();
    } else {
        fn_1_B870();
    }
}
