#include "types.h"


struct OSCreateAlarm_Arg0 {
    u32 unk_0;
    u32 unk_4;
};

void OSCreateAlarm(struct OSCreateAlarm_Arg0 *arg0) {
    arg0->unk_0 = 0;
    arg0->unk_4 = 0;
}
