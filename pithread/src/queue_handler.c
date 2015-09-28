/*
 * queue_handler.c
 *
 *  All methods on this class begin with an queue_ and they perform operations on the given parameters.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/pidata.h"


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
      printf("||: ");
      if (new_tcb->prev) printf("%d ", new_tcb->prev->tid);
      printf("<- %d ", new_tcb->tid);
      if (new_tcb->next) printf("-> %d\n", new_tcb->next->tid);
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

bool queue_has_thread_with_id(TCB_queue_t *queue, int thread_id) {
 
  if ((queue == NULL) || (queue->start == NULL && queue->end == NULL)) {
      return false;
  } else if (queue->start == NULL || queue->end == NULL) {
      printf("Something is wrong... #7\n");
      return false;
  } else {

    TCB_t *temp = queue->start;
    
    while(temp != NULL){
      if(temp->tid == thread_id){
        return true;
      }
      temp = temp->next;
    }
    return false;
  }
}



/*----------------------------------------------------------------------------*/

void list_remove(TCB_queue_t *list, TCB_t *node) {
    if(node == list->start && node == list->end) {
        list->start = NULL;
        list->end = NULL;
    } else if(node == list->start) {
        list->start = node->next;
        list->start->prev = NULL;
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
        list_remove(list, thread);
        return true;
      }
      temp = temp->next;
    }
  }
  return false;
}

