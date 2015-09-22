/*
 * threads_persistence.h
 *
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
 
#include "../include/pidata.h"
#include "queue_handler.c"

#define MAX_THREAD_PRIORITY 100


TCB_queue_t *ready_active[MAX_THREAD_PRIORITY];
TCB_queue_t *ready_expired[MAX_THREAD_PRIORITY];
TCB_queue_t *blocked_list;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

void insert_ready_active(TCB_t *thread, int position) {
	int priority = thread->credCreate;
	queue_insert(&ready_active[position], thread);
}

/*----------------------------------------------------------------------------*/

TCB_t* get_ready_active() {
	int priority = 2;
	return queue_remove(ready_active[priority]);
}

/*----------------------------------------------------------------------------*/

void insert_ready_expired(TCB_t *thread, int position) {
	int priority = thread->credCreate;
	queue_insert(&ready_expired[position], thread);
}

/*----------------------------------------------------------------------------*/

TCB_t* get_ready_expired() {
	int priority = 0;
	return queue_remove(ready_expired[priority]);
}


/*----------------------------------------------------------------------------*/

void insert_in_blocked_list(TCB_t *thread) {
	queue_insert(&blocked_list, thread);
}

/*----------------------------------------------------------------------------*/

void remove_from_blocked_list(TCB_t *thread) {
	remove_from_list(blocked_list, thread);
}





/*----------------------------------------------------------------------------*/


void printAllQueues() {
	printf("\n                            START PRINTING QUEUES:\n");
	printf("\n-> Ready-Active");
	printf("\n                                                            -> Ready-Expired");
	printf("\n                                                                                                                       -> Blocked-List");
	TCB_t *blocked_pointer = NULL;
	if (blocked_list != NULL) { blocked_pointer = blocked_list->top; }
	for (int i = 0; i<MAX_THREAD_PRIORITY; i++) {
		printf("\n[%02d]: ", i);
		if (ready_active[i] != NULL) {
			TCB_t *temp = ready_active[i]->top;
			while(temp != NULL) {
				printf("%d -:- ", temp->tid);
				temp = temp->next;
			}
			printf("fim.");
		} else {
			printf("(not initialized)");
		}

		printf("\n                                                            [%02d]: ", i);
		if (ready_expired[i] != NULL) {
			TCB_t *temp = ready_expired[i]->top;
			while(temp != NULL) {
				printf("%d -:- ", temp->tid);
				temp = temp->next;
			}
			printf("fim.");
		} else {
			printf("(not initialized)");
		}


		printf("\n                                                                                                                       [%02d]: ", i);
		if (blocked_pointer != NULL) {
			printf("%d -:- ", blocked_pointer->tid);
			blocked_pointer = blocked_pointer->next;
		} else {
			printf("Empty");
		}
	}
	printf("\nend;\n");
}