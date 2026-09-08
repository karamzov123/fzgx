#include "types.h"

/* fzgx:begin fn_1_9E194 */
extern void fn_1_9DDA8(u32, u32, u32);

void fn_1_9E194(u32 arg0, u32 arg1) {
    fn_1_9DDA8(arg0, arg1, 1);
}
/* fzgx:end fn_1_9E194 */

/* fzgx:begin fn_1_9E5B8 */
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
/* fzgx:end fn_1_9E5B8 */
