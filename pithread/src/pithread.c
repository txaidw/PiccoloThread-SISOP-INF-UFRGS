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

	printAllReadyActiveQueue();
	initializeQueue(&ready_active[7]);
	printAllReadyActiveQueue();
	TCB_t *um = (TCB_t*) malloc(sizeof(TCB_t));
	um->tid = 1;

	initializeQueue(&ready_active[0]);
	insert_ready_active(um);

	TCB_t *dois = (TCB_t*) malloc(sizeof(TCB_t));
	dois->tid = 2;
	insert_ready_active(dois);

	TCB_t *tres = (TCB_t*) malloc(sizeof(TCB_t));
	tres->tid = 3;
	insert_ready_active(tres);
	
	printAllReadyActiveQueue();

	get_ready_active();
	printAllReadyActiveQueue();
	return 0;
}