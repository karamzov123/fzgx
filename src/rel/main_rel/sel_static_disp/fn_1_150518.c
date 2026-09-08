#include "types.h"
#include "rel/main_rel/globals.h"

extern void fn_1_9A508(Obj_1_data_2A7E0 *state);
extern void fn_1_150C8C(Obj_1_data_2A7E0_At3C *obj);
extern void fn_1_150F30(Obj_1_data_2A7E0_At3C *obj);

// Reset the active object's counters and re-run its two setup passes.
void fn_1_150518(void) {
    Obj_1_data_2A7E0_At3C *obj = lbl_1_data_2A7E0.unk_3C;

    obj->unk_84 = 0;
    obj->unk_2724 = 0;
    fn_1_9A508(&lbl_1_data_2A7E0);
    fn_1_150C8C(obj);
    fn_1_150F30(obj);
    obj->unk_0 = 1;
}
