#include "types.h"

extern void __init_cpp(void);

void __init_user(void) {
    __init_cpp();
}
