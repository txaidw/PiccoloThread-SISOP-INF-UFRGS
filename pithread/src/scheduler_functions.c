
#include "../include/pidata.h"

#define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>




TCB_t* get_new_thread() {
	TCB_t *first_thread = ready_queue_remove_and_return();
	if (first_thread == NULL) {
		PIPRINT(("ERROR_CODE: first_thread == NULL\n"));
		return NULL;
	} else {
		return first_thread;
	}
}



int run_scheduler() {
	/* Remove current thread from the ready queue. */
	volatile bool already_swapped_context = false;

	// Leaving the Execution
	if (current_running_thread != NULL) {
		// There is a thread active
		switch(current_running_thread->state) {
			case PI_EXEC: {
				/// current_running_thread was executing but now should be
				/// re-inserted on the ready queue (Usually after a Yield).
				
				PIPRINT(("[PI]: Thread (%d) loose #10 credits\n", current_running_thread->tid));
				int cred = current_running_thread->credReal;
				cred = cred-10;
				current_running_thread->credReal = (cred > 0 ? cred : 0);

				ready_queue_insert(current_running_thread);
				getcontext(&(current_running_thread->context));
				break;
			}
			case PI_FINISHED:
				PIPRINT(("[PI] [SC] NOO\n"));
				break;
			case PI_BLOCKED: {
				PIPRINT(("[PI] [SC]: Blocking thread with id: %d\n", current_running_thread->tid));
				getcontext(&(current_running_thread->context));
				break;
			}
			case PI_CREATION:
				PIPRINT(("[PI] [SC]: First time the thread ran\n"));
				break;
			case PI_READY:
				PIPRINT(("[PI] [SC]: Thread wasnt running\n"));
				break;
		}
	}
	
	// Back to Execution
	if (!already_swapped_context) {
		already_swapped_context = true;

		current_running_thread = get_new_thread(); 

		if (current_running_thread == NULL || current_running_thread->tid < 0) {
			PIPRINT(("[PI] [SC]: ERROR_CODE: f_thread == NULL || f_thread->tid < 0\n"));
			return ERROR_CODE;
		} else {
			PIPRINT(("[PI] [SC]: Thread %d is active now\n", current_running_thread->tid));
			current_running_thread->state = PI_EXEC;
    		setcontext(&(current_running_thread->context));
    		return SUCESS_CODE;
		}
	}
}



/*----------------------------------------------------------------------------*/

void end_thread_execution() {

	if(current_running_thread != NULL){
		int runnig_tid = current_running_thread->tid;

		PIPRINT(("[PI]: Thread %d is beeing released\n", runnig_tid));
		current_running_thread->state = PI_FINISHED;

		free(current_running_thread);
		current_running_thread = NULL;
		// Search for a thread that is waiting for this one to finish.
		TCB_t *waiting_thread = thread_blocked_waiting_for(runnig_tid);
		// There was a thread waiting for it to finish
		if (waiting_thread != NULL) {

			blocked_tid_list_remove(waiting_thread->tid);
			blocked_list_wait_remove(waiting_thread);
			waiting_thread->state = PI_READY;

			PIPRINT(("[PI]: Thread (%d) receive #20 credits\n", waiting_thread->tid));
			int cred = waiting_thread->credReal;
			cred = cred+20;
			waiting_thread->credReal = (cred < 100 ? cred : 100);
			
			ready_queue_insert(waiting_thread);
			run_scheduler();
		} else {
			run_scheduler();
		}
	} else {
		PIPRINT(("ERROR_CODE: No thread in the queue;"));
	}
}