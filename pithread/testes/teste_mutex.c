#include "../include/pithread.h"
#include <stdio.h>

int teste = 0;
pimutex_t mut;

void func0(void *arg) {
    printf("Eu sou a thread ID0 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mut);
    teste = teste + 1;
    printf("\nID0: Vou dar um piyield dentro da SC\n");
    piyield();
    printf("\nID0: Vou liberar a SC\n");
    piunlock(&mut);
    printf("Fim da ID0\n");
    return;
}

void func1(void *arg) {
    printf("Eu sou a thread ID1 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mut);
    teste = teste + 1;
    printf("\nID1: Vou dar um piyield dentro da SC\n");
    piyield();
    printf("\nID1: Vou liberar a SC\n");
    piunlock(&mut);
    printf("Fim da ID1\n");
    return;
}

void func2(void *arg) {
    printf("Eu sou a thread ID2 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mut);
    teste = teste + 1;
    printf("\nID2: Vou dar um piyield dentro da SC\n");
    piyield();
    printf("\nID2: Vou liberar a SC\n");
    piunlock(&mut);
    printf("Fim da ID2\n");
    return;
}

void func3(void *arg) {
    printf("Eu sou a thread ID3 imprimindo %d e entrando na pilock\n", *((int *)arg));

    pilock(&mut);
    teste = teste + 1;
    printf("\nID3: Vou dar um piyield dentro da SC\n");
    piyield();
    printf("\nID3: Vou liberar a SC\n");
    piunlock(&mut);
    printf("Fim da ID3\n");
    return;
}

int main(int argc, char *argv[]) {
    int id0, id1, id2, id3;
    int i = 0;

    pimutex_init(&mut);

    id0 = picreate(90, (void*(*)(void*))func0, (void *)&i);
    id1 = picreate(90, (void*(*)(void*))func1, (void *)&i);
    id2 = picreate(80, (void*(*)(void*))func2, (void *)&i);
    id3 = picreate(70, (void*(*)(void*))func3, (void *)&i);


    piwait(id0);
    piwait(id1);
    piwait(id2);
    piwait(id3);


    printf("Eu sou a main voltando para terminar o programa e meu teste eh de %d\n", teste);

    return 0;
}