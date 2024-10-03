#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>

int main(void){
    int chave = 1112, seq = 0, *memoria_m2;

    int shmid = shmget(chave, 2*sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    memoria_m2 = (int*)shmat(shmid, NULL, 0);
    if (memoria_m2 == (int*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sleep(rand() % 5);
    memoria_m2[0] = rand() % 50;
    memoria_m2[1] = seq + 1;

    return 0;
}