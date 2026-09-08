#include "types.h"

typedef struct fn_1_547F8_node {
    struct fn_1_547F8_node *next;
    void (*callback)(struct fn_1_547F8_node *);
} fn_1_547F8_node;

void fn_1_547F8(fn_1_547F8_node *node) {
    while (node != 0) {
        if (node->callback != 0) {
            node->callback(node);
        }
        node = node->next;
    }
}
