#include "types.h"

extern u32 fn_80072C24(u32, u32, u32, u32, u32);
extern u32 fn_80072CC4(u32, u32, u32, u32, u32);
extern u32 fn_80072D64(u32, u32, u32, u32, u32, u32);
extern u32 fn_80072E20(u32, u32, u32, u32, u32, u32);
extern u32 fn_800734A8(u32, u32, u32, u32);
extern u32 fn_800735C8(u32, u32);
extern u32 fn_80073620(u32, u32);
extern u32 fn_80073C6C(u32);

void fn_80077654(u32 arg0) {
    fn_800735C8(arg0, 14);
    fn_80073620(arg0, 30);
    fn_80073C6C(arg0);
    fn_800734A8(arg0, 255, 255, 255);
    fn_80072C24(arg0, 15, 0, 14, 15);
    fn_80072D64(arg0, 0, 0, 0, 1, 0);
    fn_80072CC4(arg0, 7, 0, 6, 7);
    fn_80072E20(arg0, 0, 0, 0, 1, 0);
}
