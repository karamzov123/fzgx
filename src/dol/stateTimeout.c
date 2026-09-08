#include "types.h"

extern void __DVDStoreErrorCode(int);
extern void DVDReset(void);
extern void cbForStateError(int);

void stateTimeout(void) {
    __DVDStoreErrorCode(0x01234568);
    DVDReset();
    cbForStateError(0);
}
