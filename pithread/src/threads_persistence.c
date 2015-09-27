/*
 * threads_persistence.h
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
 
#include "../include/pidata.h"
#include "queue_handler.c"

#define MAX_THREAD_PRIORITY 100


TCB_queue_t *ready_active[MAX_THREAD_PRIORITY];
TCB_queue_t *ready_expired[MAX_THREAD_PRIORITY];
TCB_queue_t *blocked_list;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

bool ready_active_is_empty() {
	// TODO
	return true;
}

/*----------------------------------------------------------------------------*/

void ready_active_insert(TCB_t *thread) {
	int newPriority = thread->credReal;
	queue_insert(&ready_active[newPriority], thread);
}

/*----------------------------------------------------------------------------*/

TCB_t* ready_active_remove_and_return() {
	int top_priority = MAX_THREAD_PRIORITY-1;
	TCB_t *higher_priority_thread = NULL;
	while(higher_priority_thread == NULL && top_priority >= 0){
		higher_priority_thread = queue_remove(ready_active[top_priority]);
		top_priority--;
	}
	return higher_priority_thread;
}

/*----------------------------------------------------------------------------*/

void ready_expired_insert(TCB_t *thread) {
	int priority = thread->credCreate;
	queue_insert(&ready_expired[priority], thread);
}

/*----------------------------------------------------------------------------*/

bool contains_tid_in_ready_queue(int tid) {
	for (int i = 0; i < MAX_THREAD_PRIORITY; i++) {
		if ((queue_has_thread_with_id(ready_active[i], tid)) ||
			(queue_has_thread_with_id(ready_expired[i], tid))) {
			return true;
		}
	}
	return false;
}

bool contains_tid_in_blocked_list(int tid) {
	if (queue_has_thread_with_id(blocked_list, tid)) {
		return true;
	} else {
		return false;
	}
}

/*----------------------------------------------------------------------------*/

void blocked_list_insert(TCB_t *thread) {
	queue_insert(&blocked_list, thread);
}

/*----------------------------------------------------------------------------*/

void blocked_list_remove(TCB_t *thread) {
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