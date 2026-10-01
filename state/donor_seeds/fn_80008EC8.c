#include "types.h"

// Donor seed for fn_80008EC8 (addr 0x80008EC8)
// Extracted from OSAllocHead.c (original name: OSInitAlloc)

void* fn_80008EC8(void* arenaStart, void* arenaEnd, int maxHeaps)
{
    unsigned int arraySize;
    int i;
    HeapDesc* hd;

    arraySize = maxHeaps * 12;
    gAssetBudgetB = (HeapDesc*)arenaStart;
    lbl_801A6740 = maxHeaps;

    for (i = 0; i < lbl_801A6740; i++) {
        hd = &gAssetBudgetB[i];
        hd->size = -1;
        hd->free = hd->allocated = 0;
    }

    g_currentHeapHandle = -1;
    arenaStart = (void*)((char*)gAssetBudgetB + arraySize);
    arenaStart = (void*)(((unsigned int)arenaStart + 0x1F) & 0xFFFFFFE0);
    lbl_801A673C = (unsigned int)arenaStart;
    lbl_801A6738 = (unsigned int)arenaEnd & 0xFFFFFFE0;
    OSAllocTableInit();
    return arenaStart;
}
