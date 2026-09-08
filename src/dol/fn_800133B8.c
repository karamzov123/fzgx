#include "types.h"

extern u32 SISetSamplingRate(u32);
extern u32 SamplingRate_801A6820;

u32 fn_800133B8(void) {
    return SISetSamplingRate(SamplingRate_801A6820);
}
