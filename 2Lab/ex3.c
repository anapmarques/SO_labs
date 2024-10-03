#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void encontra_posicao(int inicio, int final, int num, int* vetor, int processo){
    for (int i = inicio; i < final; i++){
        if (vetor[i] == num){
            printf("O processo %d encontrou a chave %d na posicao %d\n", processo, num, i);
        }
    }
}

int main (void){
    int num_processos = 4;
    int chave = 1111;
    int *memoria_compartilhada;
    int tam_vetor = 100;
    int vetor[tam_vetor];
    int shmid;
    int divisao = tam_vetor / num_processos;
    int num = 29;

    for (int i = 0; i < tam_vetor; i++){
        vetor[i] = rand() % 100;
    }

    shmid = shmget(chave, tam_vetor * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    memoria_compartilhada = (int*)shmat(shmid, NULL, 0);
    if (memoria_compartilhada == (int*)-1) {
        perror("shmat");
        exit(1);
    }
    
    for (int i = 0; i < tam_vetor; i ++){
        *(memoria_compartilhada + i) = vetor[i];
    }
    
    // Calcular o tamanho da parte do vetor que cada processo vai buscar
    for (int i = 0; i < num_processos; i++){
        int inicio = divisao * i;
        int final = inicio + divisao;

        int pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Processo filho
            encontra_posicao(inicio, final, num, memoria_compartilhada, i);
            exit(EXIT_SUCCESS);
        }
    }

    // Espera todos os processos filhos terminarem
    for (int i = 0; i < num_processos; i++) {
        wait(NULL);
    }

    // Desanexa o segmento de memória compartilhada
    if (shmdt(memoria_compartilhada) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Destrói o segmento de memória compartilhada
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}