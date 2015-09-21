/*
 * threads_persistence.h
 *
 *
 *
 */

#include "../include/pidata.h"
#include "queue_handler.c"

#define MAX_THREAD_PRIORITY 100

TCB* ready_active[MAX_THREAD_PRIORITY];

void insert_ready_active(TCB *thread) {
	int priority = 0;
	queue_insert(ready_active[priority], thread);
}

/*----------------------------------------------------------------------------*/

TCB* get_ready_active() {
	int priority = 0;
	return queue_remove(ready_active[priority]);
}
