/*
 * queue_handler.c
 *
 *  All methods on this class begin with an queue_ and they perform operations on the given parameters.
 *
 */

#include <stdio.h>
#include "../include/pidata.h"

void queue_insert(TCB *queue_top, TCB *thread) {
	TCB *temp_tcb = queue_top;
	while (temp_tcb->prev != NULL) {
		temp_tcb = temp_tcb->prev;
	}
	thread->next = temp_tcb;
	temp_tcb->prev = thread;
}

/*----------------------------------------------------------------------------*/

TCB* queue_remove(TCB* queue_top) {
	TCB *first = queue_top;
	queue_top->prev->next = NULL;
	return first;
}
