#include "types.h"

typedef struct {
    s16 value;
    s16 sort_key;
} Fn1_14E8A4Entry;

int fn_1_14E8A4(const Fn1_14E8A4Entry *arg0, const Fn1_14E8A4Entry *arg1) {
    if (arg0->sort_key == arg1->sort_key) {
        return arg0->value - arg1->value;
    }
    return arg0->sort_key - arg1->sort_key;
}
