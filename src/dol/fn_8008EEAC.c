#include "types.h"

extern void fn_8008EDB0(void);
extern void fn_8008EB8C(void (*callback)(void));
extern void fn_8008ED30(void);

void fn_8008EEAC(void) {
    fn_8008EB8C(fn_8008EDB0);
    fn_8008ED30();
}
