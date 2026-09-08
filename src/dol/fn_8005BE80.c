#include "types.h"

typedef struct {
    int unk00;
    int unk04;
} Fn8005BE80Object;

int fn_8005BE80(Fn8005BE80Object* object) {
    if (object == 0) {
        return 0;
    }
    return object->unk04;
}
