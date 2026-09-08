#include "types.h"

extern u32 fn_8005B0C4(u32, u32);
extern u32 fn_8005B264(u32, u32);
extern u32 fn_8005B464(u32);

void fn_8004EEE4(u32 arg0) {
    fn_8005B0C4(arg0, 0);
    fn_8005B264(arg0, 0);
    fn_8005B464(arg0);
}
