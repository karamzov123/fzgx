#include "types.h"

extern char lbl_1_data_6700[7];
extern char lbl_1_data_6708[7];
extern void fn_8008069C(char *buffer, char *format, ...);
extern void fn_1_465D0(char *value, u32 flag);

void fn_1_41134(void *unused, char *value) {
    char buffer[128];

    fn_8008069C(buffer, lbl_1_data_6700, value);
    fn_1_465D0(buffer, 1);
    fn_8008069C(buffer, lbl_1_data_6708, value);
    fn_1_465D0(buffer, 1);
}
