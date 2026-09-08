#include "types.h"

extern char lbl_1_data_4C900[46];
extern int lbl_1_bss_8F588[188];

extern void OSReport(const char *format, ...);
extern void fn_1_465D0(void *object, int value);
extern void fn_80008BEC(void *dst, int value, int size);

typedef struct {
    void *object;
    s16 *indices;
    u8 data[0x20];
    s16 state;
    s16 state2;
    u8 type;
    u8 pad2d[0x0b];
    int value;
} Entry;

extern Entry lbl_1_data_4C810[4];

void fn_1_1594AC(int index, int flag) {
    Entry *entry;
    s16 value;
    int offset;

    entry = &lbl_1_data_4C810[index];
    if (entry->type >= 0x10) {
        OSReport(lbl_1_data_4C900);
    }

    if (flag != 0) {
        fn_1_465D0(entry->object, 6);
    } else {
        fn_1_465D0(entry->object, 5);
    }

    offset = 0;
    for (;;) {
        value = *(s16 *)((u8 *)entry->indices + offset);
        if (value == -1) {
            break;
        }
        lbl_1_bss_8F588[value] = index;
        offset += 2;
    }

    fn_80008BEC(entry->data, 0, 0x20);
    entry->state = 1;
    entry->state2 = 0;
    entry->value = flag;
}
