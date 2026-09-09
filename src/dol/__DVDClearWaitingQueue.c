#include "types.h"

typedef struct DVDCommandBlock {
    struct DVDCommandBlock* next;
    struct DVDCommandBlock* prev;
} DVDCommandBlock;

extern DVDCommandBlock WaitingQueue_8015CED8[];

void __DVDClearWaitingQueue(void) {
    u32 i;
    DVDCommandBlock* q;
    for (i = 0; i < 4; i++) {
        q = &WaitingQueue_8015CED8[i];
        q->next = q;
        q->prev = q;
    }
}
