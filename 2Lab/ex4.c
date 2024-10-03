#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>

int main(void){
    int shmid1, shmid2, *memoria_m1, *memoria_m2, chave1=1111, chave2=1112;
    int p1 = 0, p2 = 0, resultado;
    bool novo_valor1 = false, novo_valor2 = false;

    shmid1 = shmget(chave1, 2*sizeof(int), IPC_CREAT | 0666);
    if (shmid1 == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    memoria_m1 = (int*)shmat(shmid1, NULL, 0);
    if (memoria_m1 == (int*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    shmid2 = shmget(chave2, 2*sizeof(int), IPC_CREAT | 0666);
    if (shmid2 == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    memoria_m2 = (int*)shmat(shmid2, NULL, 0);
    if (memoria_m2 == (int*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) { //Filho
        execl("./p1", "./p1", NULL);
    }

    if (fork() == 0) {
        execl("./p2", "./p2", NULL);
    }

    while (1){
        if (memoria_m1[1] > p1){
            p1 = memoria_m1[1];
            novo_valor1 = true;
            printf("Valor gerado pelo processo 1: %d\n", memoria_m1[0]);
        }
        if (memoria_m2[1] > p2){
            p2 = memoria_m2[1];
            novo_valor2 = 1;
            printf("Valor gerado pelo processo 2: %d\n", memoria_m2[0]);
        }
        if(novo_valor1 && novo_valor2){
            resultado = memoria_m1[0] * memoria_m2[0];
            printf("O produto dos valores gerados: %d\n", resultado);
            break;
        }
    }

    if (shmdt(memoria_m1) == -1){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    if (shmdt(memoria_m2) == -1){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}