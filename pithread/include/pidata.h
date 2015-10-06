/*
 * pidata.h: arquivo de inclusão de uso apenas na geração da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 *
 */

#define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

 
#ifndef __pidata__
#define __pidata__

/*----------------------------------------------------------------------------*/
#define MAX_THREAD_PRIORITY 100

#define SUCESS_CODE 0
#define ERROR_CODE -1
#define MTX_LOCKED 0
#define MTX_UNLOCKED 1

#define PI_CREATION 0
#define PI_READY 1
#define PI_EXEC 2
#define PI_BLOCKED 3
#define PI_FINISHED 4

#define PIPRINT(X) //printf X /* <--- For DEBUG uncommet the print!

/*----------------------------------------------------------------------------*/

/* NÃO ALTERAR ESSA struct */
typedef struct TCB { 
	int	tid; 			// identificador da thread
	int	state;			// estado em que a thread se encontra
					// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado e 4: Término
	int	credCreate;		// créditos de criação da thread (atribuídos na criação)
	int	credReal; 		// créditos atuais da thread (usado para determinar a prioridade atual da thread)
	ucontext_t 	context;	// contexto de execução da thread (SP, PC, GPRs e recursos) 
	struct TCB 	*prev; 		// ponteiro para o TCB anterior da lista
	struct TCB 	*next; 		// ponteiro para o próximo TCB da lista
} TCB_t; 

/* Essa struct representa as pilhas de TCBs da biblioteca */
typedef struct TCB_queue {
  TCB_t* start;
  TCB_t* end;
} TCB_queue_t;

/* Essa struct representa um nodo de uma fila de TCBs bloqueados da biblioteca */
typedef struct TCB_waiting {
	int blocked_thread_id;
	int waiting_for_thread_id;
	struct TCB_waiting *next;
} TCB_waiting_t;



#endif


/*----------------------------------------------------------------------------*/
void initializeQueue(TCB_queue_t **queue);
/*----------------------------------------------------------------------------*/
void queue_insert(TCB_queue_t **queue, TCB_t *new_tcb);
/*----------------------------------------------------------------------------*/
void blocked_tid_list_insert(TCB_waiting_t *entry);
/*----------------------------------------------------------------------------*/
void blocked_tid_list_remove(int blocked_id);
/*----------------------------------------------------------------------------*/
bool blocked_tid_list_contains(int tid);
/*----------------------------------------------------------------------------*/
TCB_t* queue_remove(TCB_queue_t *queue);
/*----------------------------------------------------------------------------*/
TCB_t* queue_return(TCB_queue_t *queue);
/*----------------------------------------------------------------------------*/
TCB_t* queue_thread_with_id(TCB_queue_t *queue, int thread_id);
/*----------------------------------------------------------------------------*/
TCB_t* thread_blocked_waiting_for(int tid);
/*----------------------------------------------------------------------------*/
void list_remove(TCB_queue_t *list, TCB_t *node);
/*----------------------------------------------------------------------------*/
bool remove_from_list(TCB_queue_t *list, TCB_t *thread);
/*----------------------------------------------------------------------------*/
bool queue_is_empty(TCB_queue_t *queue);
/*----------------------------------------------------------------------------*/
bool ready_queue_is_empty();
/*----------------------------------------------------------------------------*/
void ready_queue_insert(TCB_t *thread);
/*----------------------------------------------------------------------------*/
void swap_queues();
/*----------------------------------------------------------------------------*/
TCB_t* ready_queue_remove_and_return();
/*----------------------------------------------------------------------------*/
TCB_t* ready_active_return();
/*----------------------------------------------------------------------------*/
void ready_expired_insert(TCB_t *thread);
/*----------------------------------------------------------------------------*/
bool contains_tid_in_ready_queue(int tid);
/*----------------------------------------------------------------------------*/
bool contains_tid_in_blocked_list(int tid);
/*----------------------------------------------------------------------------*/
void blocked_list_mutex_insert(TCB_t *thread);
/*----------------------------------------------------------------------------*/
void blocked_list_mutex_remove(TCB_t *thread);
/*----------------------------------------------------------------------------*/
void blocked_list_wait_insert(TCB_t *thread);
/*----------------------------------------------------------------------------*/
void blocked_list_wait_remove(TCB_t *thread);
/*----------------------------------------------------------------------------*/
void printAllQueues();
/*----------------------------------------------------------------------------*/
void debug_print_foward();
/*----------------------------------------------------------------------------*/
void debug_print_reversed();