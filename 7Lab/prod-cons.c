#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BUFFER_SIZE 16

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

// inicializa o valor do semáforo
void set_sem_value(int semId, int semNum, int value);
// operação V
void sem_signal(int semId, int semNum);
//operação P
void sem_wait(int semId, int semNum);

int main (int argc, char * argv[]) {
    char *buffer;
    int semId, shmId;

    shmId = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(char) + sizeof(int), IPC_CREAT | 0666);

    semId = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);

    set_sem_value(semId, 0, 1);
    set_sem_value(semId, 1, 0);

    //anexa memoria compartilhada
    buffer = (char *)shmat(shmId, NULL, 0);

    int pid = fork();

    if(pid == 0){ //Filho impressor
        while(1){
            sem_wait(semId, 1); //espera o buffer estar cheio

            printf("Buffer cheio, imprimindo:\n");
            for (int i = 0; i < BUFFER_SIZE; i++){
                putchar(buffer[i]); fflush(stdout);
            }
            printf("\n");

            sem_signal(semId, 0); //sinaliza que o buffer está vazio
        }
    }
    else{
        while(1){
            sem_wait(semId, 0); // espera até que o buffer esteja vazio

            int count = 0;
            char ch;
            printf("Aguardando mensagem:\n");
            while(count < BUFFER_SIZE) {
                ch = getchar();
                buffer[count] = ch;
                count++;
            }

            sem_signal(semId, 1); // sinaliza que o buffer esta cheio
        }
    }

    shmdt(buffer);
    shmctl(shmId, IPC_RMID, NULL);
    semctl(semId, 0, IPC_RMID);

    return 0;
}

void set_sem_value(int semId, int semNum, int value){
    union semun semUnion;
    semUnion.val = value;
    semctl(semId, semNum, SETVAL, semUnion);
}

void sem_wait(int semId, int semNum){
    struct sembuf semB;
    semB.sem_num = semNum;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
}

void sem_signal(int semId, int semNum){
    struct sembuf semB;
    semB.sem_num = semNum;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
}
