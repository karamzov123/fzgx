#include "types.h"

typedef struct {
    s32 value;
    s32 unk04;
} Fn147A60Entry;

extern Fn147A60Entry lbl_1_data_67F0[240];
extern void fn_1_48140(s32 value);
extern void fn_1_4DDC0(void);
extern void fn_1_4F724(void);

void fn_1_47A60(s16 index) {
    Fn147A60Entry *entry;
    s32 i;

    entry = &lbl_1_data_67F0[index * 10];
    for (i = 0; i < 10; i++, entry++) {
        if (entry->value != -1) {
            fn_1_48140(entry->value);
        } else {
            fn_1_4DDC0();
            fn_1_4F724();
            break;
        }
    }
}

