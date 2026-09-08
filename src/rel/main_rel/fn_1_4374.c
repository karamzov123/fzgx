#include "types.h"

typedef struct Node Node;
struct Node {
    u32 field_0;
    u32 field_4;
    Node *next;
    Node *prev;
};

void fn_1_4374(Node **list, Node *node) {
    Node *prev;
    Node *next;

    next = node->next;
    prev = node->prev;
    prev->next = next;
    if (next != 0) {
        next->prev = prev;
    }
    node->next = *list;
    node->field_0 = 0;
    *list = node;
}
