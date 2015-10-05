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

#define PIPRINT(X) printf X

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

typedef struct TCB_queue {
  TCB_t* start;
  TCB_t* end;
} TCB_queue_t;

typedef struct TCB_waiting {
	int blocked_thread_id;
	int waiting_for_thread_id;
	struct TCB_waiting *next;
} TCB_waiting_t;

/*----------------------------------------------------------------------------*/
void queue_insert(TCB_queue_t **queue, TCB_t *new_tcb);
TCB_t* queue_remove(TCB_queue_t *queue);
bool ready_active_is_empty();
void ready_queue_insert(TCB_t *thread);
TCB_t* ready_queue_remove_and_return();
TCB_t* ready_active_return();
void ready_expired_insert(TCB_t *thread);
TCB_t* thread_blocked_waiting_for(int tid);
bool contains_tid_in_ready_queue(int tid);
bool contains_tid_in_blocked_list(int tid);
void blocked_list_insert(TCB_t *thread);
void blocked_list_remove(TCB_t *thread);
void printAllQueues();
/*----------------------------------------------------------------------------*/

#endif