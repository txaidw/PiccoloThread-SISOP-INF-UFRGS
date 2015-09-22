/*
 * queue_handler.c
 *
 *  All methods on this class begin with an queue_ and they perform operations on the given parameters.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/pidata.h"


void initializeQueue(TCB_queue_t **queue) {
    if (*queue == NULL) {
        *queue = (TCB_queue_t *) malloc(sizeof(TCB_queue_t));
        (*queue)->top = NULL;
        (*queue)->bottom = NULL;
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

    if ((*queue)->top == NULL && (*queue)->bottom == NULL) {
      (*queue)->top = (*queue)->bottom = new_tcb;
    } else if ((*queue)->top == NULL || (*queue)->bottom == NULL) {
        printf("Something is wrong... #1\n");
    } else {
      (*queue)->bottom->next = new_tcb;
      (*queue)->bottom = new_tcb;
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




/*----------------------------------------------------------------------------*/

bool remove_from_list(TCB_queue_t *list, TCB_t *thread) {
  if (list != NULL) {
    TCB_t *temp = list->top;
    while(temp != NULL) {  
      if (temp == thread) {  /* Found it. */
        if (temp->prev == NULL) {
          /* Remove from beginning */
          list->top = temp->next;
        } else if (temp->next == NULL) {
          /* Remove from end */
          temp->prev->next = NULL;
        } else {
          /* Remove from middle */
          temp->prev->next = temp->next;
          temp->next->prev = temp->prev;
        }
        return true;
      }
      temp = temp->next;
    }
  }
  return false;
}