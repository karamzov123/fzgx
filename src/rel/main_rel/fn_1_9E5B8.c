#include "types.h"

typedef struct Node Node;
struct Node {
    Node *next;
};

extern Node *lbl_1_bss_6EA80;
extern Node *lbl_1_bss_6EA84;

void fn_1_9E5B8(Node *node) {
    Node **link;

    link = &lbl_1_bss_6EA80;
    while (*link != 0 && *link != node) {
        link = (Node **)*link;
    }
    if (*link == 0) {
        return;
    }
    *link = node->next;
    if (lbl_1_bss_6EA84 == node) {
        lbl_1_bss_6EA84 = (Node *)link;
    }
}
