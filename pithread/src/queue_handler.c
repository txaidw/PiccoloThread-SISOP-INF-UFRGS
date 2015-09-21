/*
 * queue_handler.c
 *
 *  All methods on this class begin with an queue_ and they perform operations on the given parameters.
 *
 */

#include <stdio.h>
#include "../include/pidata.h"


void queue_insert(TCB_queue_t *queue, TCB_t *new_tcb) {
	new_tcb->prev = NULL;
	new_tcb->next = NULL;
 
    if (queue == NULL) {
        printf("Queue not initialized\n");
    } else if (queue->top == NULL && queue->bottom == NULL) {
      queue->top = queue->bottom = new_tcb;
    } else if (queue->top == NULL || queue->bottom == NULL) {
        printf("Something is wrong... #1\n");
    } else {
      queue->bottom->next = new_tcb;
      queue->bottom = new_tcb;
    }
}


/*----------------------------------------------------------------------------*/

TCB_t* queue_remove(TCB_queue_t *queue) {
 
  if ((queue == NULL) || (queue->top == NULL && queue->bottom == NULL)) {
      printf("List is empty\n");
      return NULL;
  } else if (queue->top == NULL || queue->bottom == NULL) {
      printf("Something is wrong... #2\n");
      return NULL;
  } else {

    TCB_t *h = queue->top;
    TCB_t *p = h->next;
    
    queue->top = p;
    if (queue->top == NULL) {
        queue->bottom = queue->top;
    }
    return h;
    }
}