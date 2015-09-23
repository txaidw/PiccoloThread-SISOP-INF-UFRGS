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



/*----------------------------------------------------------------------------*/
#define SUCESS_CODE 1
#define ERROR_CODE -1
#define MTX_LOCKED 0
#define MTX_UNLOCKED 1

#define PI_CREATION 0
#define PI_READY 1
#define PI_EXEC 2
#define PI_BLOCKED 3
#define PI_FINISHED 4


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// int make_new_id() {
// 	int id = global_thread_id;
// 	++global_thread_id;
// 	return id;
// }
TCB_t* this_thread() {
	// TODO
	return NULL;
}

void run_scheduler() {
	// TODO
}



/*----------------------------------------------------------------------------*/
/*                                  BLOCKING                                  */
/*----------------------------------------------------------------------------*/

void thread_block(TCB_t* thread) {
	// TODO
	if (thread != NULL) {
		thread->state = PI_BLOCKED;
	}
	run_scheduler();
}

void thread_unblock(TCB_t* thread) {
	// TODO
	if (thread != NULL) {
		remove_from_blocked_list(thread);
		thread->state = PI_READY;
		insert_ready_active(thread);
	}
	run_scheduler();
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

int picreate (int credCreate, void* (*start)(void*), void *arg) {
	// TODO
	return -99;
}

int piyield(void) {
	// TODO
	return -99;
}

int piwait(int tid) {
	// TODO
	return -99;
}




/*----------------------------------------------------------------------------*/
/*								     MUTEX       							  */
/*----------------------------------------------------------------------------*/

int pimutex_init(pimutex_t *mtx) {
	// Initializing mutex
	if(mtx != NULL){
		mtx->flag = false;
		mtx->first = NULL;
		mtx->last = NULL;
		return SUCESS_CODE;
	} else {
		return ERROR_CODE;
	}
}

/*----------------------------------------------------------------------------*/

int pilock (pimutex_t *mtx) {
	if(mtx != NULL){
		if (mtx->flag == MTX_LOCKED) {
			// The resouce is ALREADY being used, so we must block the thread.
			TCB_t *thread = this_thread();
			// TODO: confirm if this // will work..,
			TCB_queue_t *tempQueue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
			tempQueue->top = mtx->first;
			tempQueue->bottom = mtx->last;
			queue_insert(&tempQueue, thread);
			mtx->first = tempQueue->top;
			mtx->last = tempQueue->bottom;
			//

			thread_block(thread);
			return SUCESS_CODE;
		} else if(mtx->flag == MTX_UNLOCKED) {
			// The resouce is NOT being used, so the thread is goint to use.
			mtx->flag = MTX_LOCKED;
			return SUCESS_CODE;
		} else {
			return ERROR_CODE;
		}
	} else {
		return ERROR_CODE;
	}
}

/*----------------------------------------------------------------------------*/

int piunlock (pimutex_t *mtx) {
	if(mtx != NULL){
		if ((mtx->flag == MTX_LOCKED) && (mtx->first != NULL)) {
			// Mutex is locked and there is threads on the blocked queue
	
			// TODO: confirm if this // will work..,
			TCB_queue_t *tempQueue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
			tempQueue->top = mtx->first;
			tempQueue->bottom = mtx->last;
			TCB_t *thread = queue_remove(tempQueue);
			mtx->first = tempQueue->top;
			mtx->last = tempQueue->bottom;
			//
			
			thread_unblock(thread);
		}

		if ((mtx->first == NULL) && (mtx->last == NULL)) {
			// Zero threads on the blocked queue
			mtx->flag = MTX_UNLOCKED;
		}

		return SUCESS_CODE;
	} else {
		return ERROR_CODE;
	}
}



/*----------------------------------------------------------------------------*/
/*								     TESTS           						  */
/*----------------------------------------------------------------------------*/

int main(int argc, char const *argv[]) {
	/* code */

	TCB_t *um = (TCB_t*) malloc(sizeof(TCB_t));
	um->tid = 1;

	insert_ready_active(um);

	TCB_t *dois = (TCB_t*) malloc(sizeof(TCB_t));
	dois->tid = 2;
	insert_ready_active(dois);
// 
	TCB_t *tres = (TCB_t*) malloc(sizeof(TCB_t));
	tres->tid = 3;
	insert_ready_expired(tres);


	TCB_t *cinco = (TCB_t*) malloc(sizeof(TCB_t));
	cinco->tid = 5;
	insert_in_blocked_list(cinco);

	printAllQueues();
	TCB_t *quatro = get_ready_active();
	remove_from_blocked_list(cinco);
	printAllQueues();

	pimutex_t *mutex = (pimutex_t *) malloc(sizeof(pimutex_t));
	pimutex_init(mutex);

	printf("\n%d", mutex->flag);

	return 0;
}