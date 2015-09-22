/*
 * pidata.h: arquivo de inclusão de uso apenas na geração da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 *
 */

 #define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
 #include <ucontext.h>

#ifndef __pidata__
#define __pidata__

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
  TCB_t* top;
  TCB_t* bottom;
} TCB_queue_t;

enum Thread_State {
        CREATION  = 0,
        READY     = 1,
        EXEC      = 2,
        BLOCKED   = 3,
        FINISHED  = 4
};


#endif