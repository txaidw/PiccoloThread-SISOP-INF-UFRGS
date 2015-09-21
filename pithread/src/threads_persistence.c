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

/*----------------------------------------------------------------------------*/

void initializeQueue(TCB_queue_t **queue) {
	if (*queue == NULL) {
		*queue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
		(*queue)->top = NULL;
		(*queue)->bottom = NULL;
	} else {
		printf("Queue already initialized");
	}
}

/*----------------------------------------------------------------------------*/

void insert_ready_active(TCB_t *thread) {
	int priority = 0;
	queue_insert(ready_active[priority], thread);
}

/*----------------------------------------------------------------------------*/

TCB_t* get_ready_active() {
	int priority = 0;
	return queue_remove(ready_active[priority]);
}


void printAllReadyActiveQueue() {
	printf("\nstart:");
	for (int i = 0; i<MAX_THREAD_PRIORITY; i++) {
		printf("\n [%d]: ", i);
		if (ready_active[i] != NULL) {
			TCB_t *temp = ready_active[i]->top;
			while(temp != NULL) {
				printf("%d -:- ", temp->tid);
				temp = temp->next;
			}
			printf(" fim.\n");
		} else {
			printf("Queue not initialized\n");
		}
	}
	printf("\nend;\n");
}