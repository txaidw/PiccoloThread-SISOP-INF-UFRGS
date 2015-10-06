/*
//   Esse Teste cria uma sequencia de threads, cada uma possui uma secao critica
//   onde eh usado o mutex. alem de possuirem uma cedencia voluntaria.
//   O resultado eh a impressao sequencial inversa das threads 3, 2 e 1 pela suas prioridades
//   imprimem o valor passado por parametro de uma variavel que eh incrementada dentro
//   da secao critica.
*/

#include "../include/pithread.h"
#include <stdio.h>

pimutex_t mtx;

void func0(void *arg) {
    printf("Eu sou a thread #1 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mtx);
    (*(int *)arg)++;
    piyield();
    piunlock(&mtx);
    printf("Fim da thread #1\n");
    return;
}

void func1(void *arg) {
    printf("Eu sou a thread #2 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mtx);
    (*(int *)arg)++;
    piyield();
    piunlock(&mtx);
    printf("Fim da thread #2\n");
    return;
}

void func2(void *arg) {
    printf("Eu sou a thread #3 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mtx);
    (*(int *)arg)++;
    piyield();
    piunlock(&mtx);
    printf("Fim da thread #3\n");
    return;
}

int main(int argc, char *argv[]) {
    int globalCounter = 0;

    pimutex_init(&mtx);

    int id0 = picreate(30, (void*(*)(void*))func0, (void *)&globalCounter);
    int id1 = picreate(50, (void*(*)(void*))func1, (void *)&globalCounter);
    int id2 = picreate(70, (void*(*)(void*))func2, (void *)&globalCounter);


    piwait(id1);
    piwait(id0);  // Inverted to test priority
    piwait(id2);

    printf("Eu sou a main voltando para terminar o programa\n");

    return 0;
}