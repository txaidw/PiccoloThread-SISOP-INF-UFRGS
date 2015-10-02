/*
 * pithread.c
 *
 *
 *
 */

#define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
#include <ucontext.h>

#include "../include/pithread.h"
#include "../include/pidata.h"
#include "pidata.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
int global_var_tid = 0;

ucontext_t *scheduler_context;
TCB_t *temp_scheduler;
ucontext_t *ending_contex;

#define PIPRINT(X) printf X


TCB_t* first_thread() {
	TCB_t *first_thread = ready_active_return();
	if (first_thread == NULL) {
		PIPRINT(("ERROR_CODE: first_thread == NULL\n"));
		return NULL;
	} else {
		return first_thread;
	}
}

int run_scheduler() {
	/* Remove current thread from the ready queue. */
	TCB_t* running_thread = first_thread();

	if (running_thread == NULL) {
		return ERROR_CODE;
	} else {
		volatile bool already_swapped_context = false;


		// Leaving the Execution
		switch(running_thread->state) {
			case PI_EXEC:
				// uthread_debug("Sending thread %i to the end of the queue\n", thread->tid);
				// thread->state = PI_READY;
				// push_ready(thread);

				// /* Save it's context. */
				// code |= getcontext((thread->context));
				PIPRINT(("[PI] [SC]: Thread is in execution\n"));
				/* When it returns, context will return right here. */
				break;
			case PI_FINISHED:
				PIPRINT(("[PI] [SC]: Thread is beeing released\n"));
				TCB_t *removed = ready_active_remove_and_return();
				free(removed);
				running_thread = NULL;
				removed = NULL;
				break;
			case PI_BLOCKED: {
				PIPRINT(("[PI] [SC]: Blocking thread with id: %d\n", running_thread->tid));
				TCB_t *blocked = ready_active_remove_and_return();
				blocked_list_insert(blocked);
				getcontext(&(blocked->context));
				break;
			}
			case PI_CREATION:
				PIPRINT(("[PI] [SC]: First time the thread ran\n"));
				break;
			case PI_READY:
				PIPRINT(("[PI] [SC]: Thread wasnt running\n"));
				break;
		}

		// Back to Execution
		if (!already_swapped_context) {
			already_swapped_context = true;

			TCB_t *back_thread = first_thread(); 

			if (back_thread == NULL || back_thread->tid < 0) {
				PIPRINT(("[PI] [SC]: ERROR_CODE: f_thread == NULL || f_thread->tid < 0\n"));
				return ERROR_CODE;
			} else {
				PIPRINT(("[PI] [SC]: Thread %d is active now\n", back_thread->tid));
				back_thread->state = PI_EXEC;
        		setcontext(&(back_thread->context));
        		return SUCESS_CODE;
			}
		}

		return SUCESS_CODE;
	}
}





/*----------------------------------------------------------------------------*/
/*                                  BLOCKING                                  */
/*----------------------------------------------------------------------------*/

int thread_block(TCB_t* thread) {
	if (thread != NULL) {
		thread->state = PI_BLOCKED;
		return run_scheduler();
	} else {
		return ERROR_CODE;
	}
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
/*                                END THREAD                                  */
/*----------------------------------------------------------------------------*/


void end_thread_execution() {
	TCB_t *thread = first_thread();
	if(thread != NULL){

		thread->state = PI_FINISHED;

		// Search for a thread that is waiting for this one to finish.
		TCB_t *waiting_thread = thread_blocked_waiting_for(thread->tid);
		// There was a thread waiting for it to finish
		if (waiting_thread != NULL) {

			waiting_thread->waiting_for_tid = ERROR_CODE;
			thread_unblock(waiting_thread);
		}
		run_scheduler();
	} else {
		PIPRINT(("ERROR_CODE: No thread in the queue;"));
	}
}

int init_end_thread_context() {
	ending_contex = (ucontext_t *) malloc(sizeof(ucontext_t));

	if (getcontext(ending_contex) != 0 || ending_contex == NULL) {
		return ERROR_CODE;
	} else {
		ending_contex->uc_stack.ss_sp = (char *) malloc(SIGSTKSZ);
		ending_contex->uc_stack.ss_size = SIGSTKSZ;
		ending_contex->uc_link = NULL;

		makecontext(ending_contex, end_thread_execution, 0);
		return SUCESS_CODE;
	}
}


int init_main_thread() {
	TCB_t *thread = (TCB_t*)malloc(sizeof(TCB_t));

	thread->tid = global_var_tid;
	thread->state = PI_CREATION;
	thread->credCreate = thread->credReal = MAX_THREAD_PRIORITY;
	thread->next = NULL;
	thread->prev = NULL;
	thread->waiting_for_tid = ERROR_CODE;
	ready_active_insert(thread);
	return getcontext(&(thread->context));
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static bool initialized = false;


int internal_init(){
	if (!initialized) {
		initialized = true;
		global_var_tid = 0;
		if (init_end_thread_context() != SUCESS_CODE) {
			PIPRINT(("ERROR_CODE: init_end_thread_context"));
			return ERROR_CODE;
		} else if(init_main_thread() != SUCESS_CODE) {
			PIPRINT(("ERROR_CODE: init_main_thread"));
			return ERROR_CODE;
		} else {
			volatile bool main_thread_created = false;
			if (!main_thread_created) {
				main_thread_created = true;
				run_scheduler();
			}
            return SUCESS_CODE;
		}
	} else {
		return SUCESS_CODE;
	}
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


int picreate(int credCreate, void* (*start)(void*), void *arg) {
	int new_tid = ++global_var_tid;
	PIPRINT(("[PI] Creating new thread with tid: %d\n", new_tid));
	TCB_t *thread = (TCB_t*)malloc(sizeof(TCB_t));
	thread->tid = new_tid;
	thread->state = PI_CREATION;
	thread->credCreate = thread->credReal = credCreate;
	thread->next = NULL;thread->prev = NULL;
	thread->waiting_for_tid = ERROR_CODE;
	getcontext(&(thread->context));
	
	if (((thread->context).uc_stack.ss_sp = malloc(SIGSTKSZ)) == NULL) {
		printf("[PI ERROR]: No memory for stack allocation!");
		return ERROR_CODE;
	} else {
		(thread->context).uc_stack.ss_size = SIGSTKSZ;
	    (thread->context).uc_link = ending_contex;
		makecontext(&(thread->context), (void (*)(void))start, 1, arg);
		ready_active_insert(thread);
		return SUCESS_CODE;
	}
}

int piyield(void) {
	if(!ready_active_is_empty()) {
		return run_scheduler();
	} else {
		return SUCESS_CODE;
	}
}

int piwait(int tid) {
	if (contains_tid_in_ready_queue(tid)) {
	    TCB_t * thread = first_thread();
	    thread->waiting_for_tid = tid;
	    return thread_block(thread);
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
			TCB_t *thread = first_thread();
			// TODO: confirm if this // will work..,
			TCB_queue_t *tempQueue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
			tempQueue->start = mtx->first;
			tempQueue->end = mtx->last;
			queue_insert(&tempQueue, thread);
			mtx->first = tempQueue->start;
			mtx->last = tempQueue->end;
			// free(tempQueue);
			// tempQueue = NULL;
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
	if (mtx != NULL){
		if ((mtx->flag == MTX_LOCKED) && (mtx->first != NULL)) {
			// Mutex is locked and there is threads on the blocked queue

			// TODO: confirm if this // will work..,
			TCB_queue_t *tempQueue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
			tempQueue->start = mtx->first;
			tempQueue->end = mtx->last;
			TCB_t *thread = queue_remove(tempQueue);
			mtx->first = tempQueue->start;
			mtx->last = tempQueue->end;
			// free(tempQueue);
			// tempQueue = NULL;
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


// int main(int argc, char const *argv[]) {
// 	/* code */
// 	printf("LIB COMPILED SUCCESSFULLY\n");
// 	return 0;
// }
