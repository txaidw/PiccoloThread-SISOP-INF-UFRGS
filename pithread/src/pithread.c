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
#define SUCESS_CODE 0
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

int run_scheduler() {
	// TODO
	return ERROR_CODE;
}



TCB_t* thread_blocked_waiting_for(int tid) {
	TCB_queue_t *queue = blocked_list;
  if ((queue == NULL) || (queue->top == NULL && queue->bottom == NULL)) {
      return NULL;
  } else if (queue->top == NULL || queue->bottom == NULL) {
      printf("Something is wrong... #7\n");
      return NULL;
  } else {
    TCB_t *temp = queue->top;
    
    while(temp != NULL) {
      if (temp->waiting_for_tid == tid){
        return temp;
      }
      temp = temp->next;
    }
    return NULL;
  }
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
		blocked_list_remove(thread);
		thread->state = PI_READY;
		ready_active_insert(thread);
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
	if(!ready_active_is_empty()) {
		return run_scheduler();
	} else {
		return SUCESS_CODE;
	}
}

int piwait(int tid) {
	if (contains_tid_in_ready_queue(tid) || contains_tid_in_blocked_list(tid)) {
	    TCB_t * thread = this_thread();
	    thread->waiting_for_tid = tid;
	    thread_block(thread);
	    return run_scheduler();
	} else {
		return SUCESS_CODE;
	}
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

	ready_active_insert(um);

	TCB_t *dois = (TCB_t*) malloc(sizeof(TCB_t));
	dois->tid = 2;
	ready_active_insert(dois);
// 
	TCB_t *tres = (TCB_t*) malloc(sizeof(TCB_t));
	tres->tid = 3;
	ready_expired_insert(tres);


	TCB_t *cinco = (TCB_t*) malloc(sizeof(TCB_t));
	cinco->tid = 5;
	blocked_list_insert(cinco);

	printAllQueues();
	TCB_t *quatro = ready_active_remove_and_return();
	blocked_list_remove(cinco);
	printAllQueues();

	pimutex_t *mutex = (pimutex_t *) malloc(sizeof(pimutex_t));
	pimutex_init(mutex);

	printf("\n%d", mutex->flag);

	return 0;
}