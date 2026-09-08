#include "types.h"

extern u32 lbl_1_bss_DAC;
extern u32 lbl_1_bss_DA8;
extern void fn_1_43A4(void);

typedef struct Node Node;
struct Node {
    u32 field_0;
    void *field_4;
    Node *next;
    u32 field_c;
    u32 field_10;
    u32 field_14;
    void (*callback)(void *, u32);
};

typedef struct List List;
struct List {
    u32 field_0;
    u32 field_4;
    Node *head;
};

typedef struct Manager Manager;
struct Manager {
    u32 field_0;
    u32 field_4;
    Node *head;
};

void fn_1_41A8(void) {
    Manager *entry;
    Manager *manager;
    u32 *dac;
    u32 *da8;
    int i;
    Node *node;
    void (*callback)(void *, u32);

    dac = &lbl_1_bss_DAC;
    manager = (Manager *)*dac;
    if (manager != 0) {
        da8 = &lbl_1_bss_DA8;
        entry = manager;
        i = 0;
        while (i < 32) {
            node = (*(List **)((u8 *)entry + 0x18))->head;
            while (node != 0) {
                callback = node->callback;
                manager->head = node;
                *dac = (u32)manager;
                *da8 = (u32)manager;
                if (callback != 0) {
                    callback(node->field_4, 1);
                }
                *dac = (u32)manager;
                *da8 = (u32)manager;
                node = node->next;
                fn_1_43A4();
            }
            i++;
            entry = (Manager *)((u8 *)entry + 4);
        }
        manager->head = 0;
        lbl_1_bss_DAC = (u32)manager;
        lbl_1_bss_DA8 = 0;
    }
}
