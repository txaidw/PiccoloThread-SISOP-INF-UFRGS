
#include "../include/pidata.h"

#define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

TCB_queue_t *ready_active[MAX_THREAD_PRIORITY];
TCB_queue_t *ready_expired[MAX_THREAD_PRIORITY];
TCB_queue_t *blocked_list_mutex;
TCB_queue_t *blocked_list_waiting;
TCB_waiting_t *blocked_tid_list;
TCB_t *current_running_thread = NULL;

/*----------------------------------------------------------------------------*/
void initializeQueue(TCB_queue_t **queue) {
    if (*queue == NULL) {
        *queue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
        (*queue)->start = NULL;
        (*queue)->end = NULL;
    } else {
        printf("Queue already initialized");
    }
}

void queue_insert(TCB_queue_t **queue, TCB_t *new_tcb) {
	 new_tcb->prev = NULL;
	 new_tcb->next = NULL;
 
    if ((*queue) == NULL) {
        initializeQueue(queue);
    }

    if ((*queue)->start == NULL && (*queue)->end == NULL) {
      (*queue)->start = (*queue)->end = new_tcb;
    } else if ((*queue)->start == NULL || (*queue)->end == NULL) {
        printf("Something is wrong... #1\n");
    } else {
      (*queue)->start->prev = new_tcb;
      new_tcb->next = (*queue)->start;
      (*queue)->start = new_tcb;
    }
}

void blocked_tid_list_insert(TCB_waiting_t *entry) {
  if(blocked_tid_list == NULL) {
      blocked_tid_list = entry;       
  } else {
    TCB_waiting_t *node = blocked_tid_list;
    
    while(node->next != NULL) {
      node = node->next;
    }
    node->next = entry;
  }
}

void blocked_tid_list_remove(int blocked_id) {

  if(blocked_tid_list == NULL) {
      blocked_tid_list = NULL;       
  } else {
    TCB_waiting_t *node = blocked_tid_list;
    TCB_waiting_t *prev = NULL;
    
    while(node != NULL) {
        if (node->blocked_thread_id == blocked_id) {
            if (prev == NULL) {
                blocked_tid_list = node->next;
            } else {
                prev->next = node->next;
            }
            free(node);
        }
        prev = node;
        node = node->next;
    }
  }
}
/*----------------------------------------------------------------------------*/

TCB_t* queue_remove(TCB_queue_t *queue) {
 
  if ((queue == NULL) || (queue->start == NULL && queue->end == NULL)) {
      return NULL;
  } else if (queue->start == NULL || queue->end == NULL) {
      printf("Something is wrong... #2\n");
      return NULL;
  } else {

    TCB_t *to_remove = queue->end;
    TCB_t *prev = to_remove->prev;

    queue->end = prev;

    if (queue->end == NULL) {
        queue->start = queue->end;
    } else {
      queue->end->next = NULL;
    }
    to_remove->prev = NULL;
    to_remove->next = NULL;
    return to_remove;
    }
}

TCB_t* queue_return(TCB_queue_t *queue) {
 
  if ((queue == NULL) || (queue->start == NULL && queue->end == NULL)) {
      return NULL;
  } else if (queue->start == NULL || queue->end == NULL) {
      printf("Something is wrong... #2\n");
      return NULL;
  } else {
    return queue->end;
  }
}

/*----------------------------------------------------------------------------*/

TCB_t* queue_thread_with_id(TCB_queue_t *queue, int thread_id) {
 
  if ((queue == NULL) || (queue->start == NULL && queue->end == NULL)) {
      return NULL;
  } else if (queue->start == NULL || queue->end == NULL) {
      printf("Something is wrong... #7\n");
      return NULL;
  } else {

    TCB_t *temp = queue->start;
    
    while(temp != NULL){
      if(temp->tid == thread_id){
        return temp;
      }
      temp = temp->next;
    }
    return NULL;
  }
}


TCB_t* thread_blocked_waiting_for(int tid) {

  if(blocked_tid_list == NULL) {
      return NULL;       
  } else {
    TCB_waiting_t *node = blocked_tid_list;
    int found_tid = -1;
    while(node != NULL) {
      if (node->waiting_for_thread_id == tid) {
        found_tid = node->blocked_thread_id;
        return queue_thread_with_id(blocked_list_waiting, found_tid);
      }
      node = node->next;
    }
    return NULL;
  }











}

/*----------------------------------------------------------------------------*/

void list_remove(TCB_queue_t *list, TCB_t *node) {
    printf("CHEGOU\n");
    if(node == list->start && node == list->end) {
        list->start = NULL;
        list->end = NULL;
    } else if(node == list->start) {
        list->start = node->next;
    } else if (node == list->end) {
        list->end = node->prev;
        list->end->next = NULL;
    } else {
        TCB_t *after = node->next;
        TCB_t *before = node->prev;
        after->prev = before;
        before->next = after;
    }
}

/*----------------------------------------------------------------------------*/

bool remove_from_list(TCB_queue_t *list, TCB_t *thread) {

  if ((list == NULL) || (list->start == NULL && list->end == NULL)) {
      return false;
  } else if (list->start == NULL || list->end == NULL) {
      printf("Something is wrong... #7\n");
      return false;
  } else {
    TCB_t *temp = list->start;
    while(temp != NULL) {  
      if (temp == thread) {  /* Found it. */
        list_remove(list, temp);
        return true;
      }
      temp = temp->next;
    }
  }
  return false;
}
/*----------------------------------------------------------------------------*/

bool queue_is_empty(TCB_queue_t *queue) {
  if ((queue == NULL) ||
     (queue->start == NULL && queue->end == NULL)) {
      return true;
  } else if (queue->start == NULL || queue->end == NULL) {
      printf("Something is wrong... #7\n");
      return false;
  } else {
      return false;
  }
}

bool ready_queue_is_empty() {
  int i;
  for (i = 0; i < MAX_THREAD_PRIORITY; i++) {
    if ((queue_is_empty(ready_active[i]) == false) ||
        (queue_is_empty(ready_expired[i]) == false)) {
      return false;
    }
  }
  return true;
}

/*----------------------------------------------------------------------------*/

void ready_queue_insert(TCB_t *thread) {
  int newPriority = thread->credReal;
  if (newPriority == 0) {
    // Should be added on the expired queue;
    thread->credReal = thread->credCreate;
    queue_insert(&ready_expired[thread->credCreate-1], thread);
  } else if (newPriority > 0) {
    // Should be added on the ready queue;
    queue_insert(&ready_active[newPriority-1], thread);
  } else {
    PIPRINT(("[PI ERROR]: Invalid Proirity"));
  }
}

/*----------------------------------------------------------------------------*/

void swap_queues() {
  int i;
  for (i = 0; i < MAX_THREAD_PRIORITY; i++) {
    TCB_t *thread = queue_remove(ready_expired[i]);
    while(thread != NULL) {
        queue_insert(&ready_active[i], thread);
        thread = queue_remove(ready_expired[i]);
    }
  }
}


TCB_t* ready_queue_remove_and_return() {

	int top_priority = MAX_THREAD_PRIORITY-1;
	TCB_t *higher_priority_thread = NULL;
	while (higher_priority_thread == NULL && top_priority >= 0) {
		higher_priority_thread = queue_remove(ready_active[top_priority]);
		top_priority--;
	}

  if (higher_priority_thread == NULL) {
    printf("[PI] Will swap queues\n");
    swap_queues();

    top_priority = MAX_THREAD_PRIORITY-1;
    while (higher_priority_thread == NULL && top_priority >= 0) {
      higher_priority_thread = queue_remove(ready_active[top_priority]);
      top_priority--;
    }
  }
	return higher_priority_thread;
}




TCB_t* ready_active_return() {
	int top_priority = MAX_THREAD_PRIORITY-1;
	TCB_t *higher_priority_thread = NULL;
	while (higher_priority_thread == NULL && top_priority >= 0){
		higher_priority_thread = queue_return(ready_active[top_priority]);
		top_priority--;
	}
	return higher_priority_thread;
}
/*----------------------------------------------------------------------------*/

void ready_expired_insert(TCB_t *thread) {
	int priority = thread->credCreate;
	queue_insert(&ready_expired[priority-1], thread);
}

/*----------------------------------------------------------------------------*/

bool contains_tid_in_ready_queue(int tid) {
  int i;
	for (i = 0; i < MAX_THREAD_PRIORITY; i++) {
		if ( queue_thread_with_id(ready_active[i], tid) != NULL ||
		     queue_thread_with_id(ready_expired[i], tid) != NULL ) {
			return true;
		}
	}
	return false;
}

bool contains_tid_in_blocked_list(int tid) {
	if (queue_thread_with_id(blocked_list_waiting, tid) != NULL ) {
		return true;
	} else {
		return false;
	}
}

/*----------------------------------------------------------------------------*/

void blocked_list_mutex_insert(TCB_t *thread) {
  queue_insert(&blocked_list_mutex, thread);
}


void blocked_list_mutex_remove(TCB_t *thread) {
  remove_from_list(blocked_list_mutex, thread);
}


void blocked_list_wait_insert(TCB_t *thread) {
	queue_insert(&blocked_list_waiting, thread);
}

/*----------------------------------------------------------------------------*/

void blocked_list_wait_remove(TCB_t *thread) {
	remove_from_list(blocked_list_waiting, thread);
}



/*----------------------------------------------------------------------------*/


void printAllQueues() {
	printf("\n                            START PRINTING QUEUES:\n");
	printf("\n-> Ready-Active");
	printf("\n                          -> Ready-Expired");
	printf("\n                                                   -> Blocked-List");
	TCB_t *blocked_pointer = NULL;
	if (blocked_list_waiting != NULL) { blocked_pointer = blocked_list_waiting->start; }
	int i;
  for (i = 0; i<MAX_THREAD_PRIORITY; i++) {
		printf("\n[%02d]: ", i);
		if (ready_active[i] != NULL) {
			TCB_t *temp = ready_active[i]->start;
			while(temp != NULL) {
				printf("%d -:- ", temp->tid);
				temp = temp->next;
			}
			printf("fim.");
		} else {
			printf("(not initialized)");
		}

		printf("\n                          [%02d]: ", i);
		if (ready_expired[i] != NULL) {
			TCB_t *temp = ready_expired[i]->start;
			while(temp != NULL) {
				printf("%d -:- ", temp->tid);
				temp = temp->next;
			}
			printf("fim.");
		} else {
			printf("(not initialized)");
		}


		printf("\n                                                   [%02d]: ", i);
		if (blocked_pointer != NULL) {
			printf("%d -:- ", blocked_pointer->tid);
			blocked_pointer = blocked_pointer->next;
		} else {
			printf("Empty");
		}
	}
	printf("\nend;\n");
}

void pf() {
	printf("\n");
	TCB_t *blocked_pointer = NULL;
	if (blocked_list_waiting != NULL) { blocked_pointer = blocked_list_waiting->start; }
	while (blocked_pointer != NULL) {
		printf("%d -:- ", blocked_pointer->tid);
		blocked_pointer = blocked_pointer->next;
	}
}

void pr() {
	printf("\n");
	TCB_t *blocked_pointer = NULL;
	if (blocked_list_waiting != NULL) { blocked_pointer = blocked_list_waiting->end; }
	while (blocked_pointer != NULL) {
		printf("%d -:- ", blocked_pointer->tid);
		blocked_pointer = blocked_pointer->prev;
	}
}