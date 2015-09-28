
/*
 *	Programa de exemplo de uso da biblioteca sthread
 *
 *	Versão 1.0 - 25/03/2015
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../src/pithread.c"
#include <stdio.h>

void* func0(void *arg) {
	printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
	return NULL;
}

void* func1(void *arg) {
	printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
    return NULL;
}

int main(int argc, char *argv[]) {
    internal_init();
    int	id0, id1;
	int i;

    id0 = picreate(1, func0, (void *)&i);
    id1 = picreate(2, func1, (void *)&i);

    printf("Eu sou a main apos a criacao de ID0 e ID1\n");

    piwait(id0);
    piwait(id1);

    printf("Eu sou a main voltando para terminar o programa\n");
}