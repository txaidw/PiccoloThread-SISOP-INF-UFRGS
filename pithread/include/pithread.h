/*
 * pithread.h: arquivo de inclus�o com os prot�tipos das fun��es a serem
 *             implementadas na realiza��o do trabalho.
 *
 * N�O MODIFIQUE ESTE ARQUIVO.
 *
 * VERS�O 1 - 17/08/2015
 */

#define _XOPEN_SOURCE 600 // Solves a OSX deprecated library problem of ucontext.h
#include <ucontext.h>

#ifndef __pithread__
#define __pithread__

typedef struct mutex {
	int	flag; 			// indica se o mutex foi adquirido ou n�o (1: livre; 0: ocupado)
	struct TCB	*first, *last; 	// ponteiros para lista de threads bloqueadas no mutex
} pimutex_t;

int picreate (int credCreate, void* (*start)(void*), void *arg);
int piyield(void);
int piwait(int tid);
int pimutex_init(pimutex_t *mtx);
int pilock (pimutex_t *mtx);
int piunlock (pimutex_t *mtx);

int internal_init();

#endif
