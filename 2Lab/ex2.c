#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define CHAVE 8752
#define TAMANHO_MEMORIA 1024

int main(void) {
    int shmid;
    char *memoria_compartilhada;
    char mensagem[TAMANHO_MEMORIA];

    // Cria ou abre um segmento de memória compartilhada
    shmid = shmget(CHAVE, TAMANHO_MEMORIA, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Anexa o segmento de memória compartilhada
    memoria_compartilhada = (char*)shmat(shmid, NULL, 0);
    if (memoria_compartilhada == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    // Lê a mensagem do stdin
    printf("Digite a mensagem do dia: ");
    fgets(mensagem, TAMANHO_MEMORIA, stdin);

    // Copia a mensagem para a memória compartilhada
    strncpy(memoria_compartilhada, mensagem, TAMANHO_MEMORIA);

    // Desanexa a memória compartilhada
    if (shmdt(memoria_compartilhada) == -1) {
        perror("shmdt");
        exit(1);
    }

    printf("Mensagem salva na memória compartilhada.\n");
    return 0;
}