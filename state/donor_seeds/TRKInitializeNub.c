#include "types.h"

// Donor seed for TRKInitializeNub (addr 0x800889B4)
// Extracted from nubinit.c (original name: TRKInitializeNub)

int TRKInitializeNub(void)
{
    int result;
    int resultTemp;

    result = TRKInitializeEndian();

    MWTRACE(1, (char*)TRKNubInitMsg_80095664);

    if (result == 0) {
        usr_put_initialize();
    }

    if (result == 0) {
        result = TRKInitializeEventQueue();
    }

    if (result == 0) {
        result = TRKInitializeMessageBuffers();
    }

    if (result == 0) {
        result = TRKInitializeDispatcher();
    }

    InitializeProgramEndTrap();

    if (result == 0) {
        result = TRKInitializeSerialHandler();
    }

    if (result == 0) {
        result = TRKInitializeTarget();
    }

    if (result == 0) {
        resultTemp = TRKInitializeIntDrivenUART(0xE100, 1, 0, (void**)gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr[0]);
        if (resultTemp != 0) {
            result = resultTemp;
        }
    }

    return result;
}
