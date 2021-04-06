#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
	node tortoise;
	node hare;
    if (head == NULL || head->next == NULL) {
        return 0;
    }

	tortoise.next = head;
	hare.next = head->next;

	do{ /*hit different nodes, acylic*/
		if (hare.next->next == NULL || hare.next == NULL) {
            return 0;
        }
        hare.next = hare.next->next->next;
        tortoise.next = tortoise.next->next;
	} while (tortoise.next != hare.next);

	return 1;
		
}