/*
 * pithread.c
 *
 *
 *
 */

#include "../include/pithread.h"
#include "../include/pidata.h"
#include <stdlib.h>
#include <stdio.h>

#include "threads_persistence.c"

int main(int argc, char const *argv[]) {
	/* code */

	TCB_t *um = (TCB_t*) malloc(sizeof(TCB_t));
	um->tid = 1;

	insert_ready_active(um, 1);

	TCB_t *dois = (TCB_t*) malloc(sizeof(TCB_t));
	dois->tid = 2;
	insert_ready_active(dois, 2);
// 
	TCB_t *tres = (TCB_t*) malloc(sizeof(TCB_t));
	tres->tid = 3;
	insert_ready_expired(tres, 3);

	TCB_t *quatro = get_ready_active();

	printf("%d\n", quatro->tid);
	TCB_t *cinco = (TCB_t*) malloc(sizeof(TCB_t));
	cinco->tid = 5;
	insert_in_blocked_list(cinco);

	printAllQueues();

	remove_from_blocked_list(cinco);
	printAllQueues();

	return 0;
}