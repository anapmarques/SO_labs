#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>

int main(void){
    int chave = 1111, seq = 0, *memoria_m1;

    int shmid = shmget(chave, 2*sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    memoria_m1 = (int*)shmat(shmid, NULL, 0);
    if (memoria_m1 == (int*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sleep(rand() % 10);
    memoria_m1[0] = rand() % 100;
    memoria_m1[1] = seq + 1;

    return 0;
}