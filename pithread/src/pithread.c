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

ucontext_t *scheduler_context;
TCB_t *temp_scheduler;
ucontext_t *ending_contex;


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
      if (temp->waiting_for_tid == tid) {
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
/*                                ERRASSEEE                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

void allocate_thread_private() {

    temp_scheduler = (TCB_t *) malloc(sizeof(TCB_t));

    // Warning: I think this is *useless*, but i'm leaving here anyway!
    // temp_scheduler->context = (ucontext_t *) malloc(sizeof(ucontext_t));



	/* Get a mold for a new context. */
    getcontext(&temp_scheduler->context);

	temp_scheduler->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	temp_scheduler->context.uc_stack.ss_size = SIGSTKSZ;

	/* The thread must go to the context that begins at terminate_current_thread
	 * when it ends. */
	temp_scheduler->context.uc_link = ending_contex;
}

/*----------------------------------------------------------------------------*/

TCB_t* allocate_thread() {
    ucontext_t *current_context;

    volatile bool already_allocated = false;

	/* Save the current context. */
    getcontext(current_context);


    if (!already_allocated)
    {
		/* Make it so it goes back to 'current_context' after it allocates. That
		 * way, it won't go inside this if anymore. */
		already_allocated = true;
        scheduler_context->uc_link = current_context;

		/* Set that the allocator_context should begin at the
		 * 'allocate_thread_private' function. */
        makecontext(scheduler_context, (void(*)(void))allocate_thread_private, 0);

        setcontext(scheduler_context);
    }

    return temp_scheduler;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                END THREAD                                  */
/*----------------------------------------------------------------------------*/


void end_thread_execution() {
	TCB_t *thread = this_thread();
	thread->state = PI_FINISHED;

	// Search for a thread that is waiting for this one to finish.

	TCB_t *waiting_thread = thread_blocked_waiting_for(thread->tid);

	// There was a thread waiting for it to finish
	if (waiting_thread != NULL) {
		waiting_thread->waiting_for_tid = ERROR_CODE;

		thread_unblock(waiting_thread);
	}
	run_scheduler();
}



// ucontext_t* create_new_context() {
// 	ucontext_t *context = (ucontext_t *) malloc(sizeof(ucontext_t));

// 	if (getcontext(context) != 0 || context == NULL) {
// 		return NULL;
// 	}

// 	/* Allocate the stack */
// 	context->uc_stack.ss_sp = (char *) malloc(SIGSTKSZ);
// 	context->uc_stack.ss_size = SIGSTKSZ;

// 	/* By default, it shouldn't return to any specified context. */
// 	context->uc_link = NULL;

// 	return context;
// }


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


void* create_main_thread(void* context) {
	/* Set the current thread (which is the main one) with the context
	 * passed to the function. */
	TCB_t *main_thread = this_thread();
	main_thread->context = *((ucontext_t *) context);
	/* Set the thread's context, to make it go back to main. */
	return (void *)setcontext(&main_thread->context);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int picreate (int credCreate, void* (*start)(void*), void *arg);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static bool initialized = false;


int internal_init(){
	if (!initialized) {
		initialized = true;

		if (init_end_thread_context() != SUCESS_CODE) {
			return ERROR_CODE;
		} else {


			if (getcontext(scheduler_context) != 0 || scheduler_context == NULL) {
				return ERROR_CODE;
			} else {

				scheduler_context->uc_stack.ss_sp = (char *) malloc(SIGSTKSZ);
				scheduler_context->uc_stack.ss_size = SIGSTKSZ;
				scheduler_context->uc_link = NULL;


				ucontext_t main_context;
				volatile bool main_thread_created = false;

				if (getcontext(&main_context) != 0) {
					return ERROR_CODE;
				} else {
					if (!main_thread_created) {
						main_thread_created = true;
						// TODO: credits;
						if (picreate(100, create_main_thread, (void*)&main_context) != SUCESS_CODE) {
							return ERROR_CODE;
						} else {
							run_scheduler();
						}
					}
				}
			}
		}
	} else {
		return SUCESS_CODE;
	}
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

int picreate (int credCreate, void* (*start)(void*), void *arg) {

	uthread_t* thread = allocate_thread(); /* Allocate a new thread */

	thread->tid = get_new_id(); /* Assign a new id to it*/
    thread->waiting_join = UTHREAD_INVALID_ID; /* It's not waiting for anybody
											   to finish yet*/

    uthread_debug("Creating new thread with id %i\n", thread->tid);

	/* Create it's context */
	makecontext((thread->context),
		(void (*) (void)) start_routine, 1, arg);

	/* Put it to the end of the ready queue */
	push_ready(thread);

	thread->state = UTHREAD_INITIALIZING;


	/* Return the newly created thread's id. */
	return thread->tid;
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
