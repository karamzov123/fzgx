#include "types.h"

extern u32 OSReport(u32, ...);
extern u32 lbl_80095BCC[];

void TRK_board_display(u32 arg0) {
    OSReport((u32)&lbl_80095BCC, arg0);
}
