#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define CHAVE 8752
#define TAMANHO_MEMORIA 1024

int main (void){
    int shmid;
    char *memoria_compartilhada;

    // Acessa o segmento de memória compartilhada
    shmid = shmget(CHAVE, TAMANHO_MEMORIA, 0666);
    if(shmid == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Anexa o segmento de memória compartilhada
    memoria_compartilhada = (char*) shmat(shmid, NULL,0);
    if (memoria_compartilhada == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    //Exibe a mensagem do dia
    printf("Mensagem do dia: %s",memoria_compartilhada);

    //Desanexa memoria compartilha 
    if (shmdt(memoria_compartilhada) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    return 0;
}