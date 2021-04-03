#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
	node tortoise;
	node hare;

	tortoise.next = head;
	hare.next = head;

	do{ /*hit different nodes, acylic*/
		hare = *hare.next->next;
		if (!hare.next) {
			return 0;
		}
		tortoise = *tortoise.next;
		
	} while (tortoise.next != hare.next);

	return 1;
		
}
