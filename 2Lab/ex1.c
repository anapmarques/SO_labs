#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define N_LINHAS  3
#define N_COLUNAS 3
#define TAMANHO_MATRIZ (N_LINHAS * N_COLUNAS)

void soma_linha(int linha, int *matriz_a, int *matriz_b, int *matriz_c);

int main(void) {
    int key = 1234; // Chave segmento de memória
    int shmid;
    int *shared_memory, *matriz_a, *matriz_b, *matriz_c;
    int valores_linhaA[] = {5, 7, 9, 6, 3, 6, 3, 1, 2};
    int valores_linhaB[] = {5, 3, 0, 6, 2, 6, 5, 7, 0};

    // Cria/abre  segmento de memória compartilhada
    shmid = shmget(key, 3 * TAMANHO_MATRIZ * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Anexa memória compartilhada no endereçamento do processo
    shared_memory = (int *)shmat(shmid, NULL, 0);
    if (shared_memory == (int *)-1) {
        perror("shmat");
        return 1;
    }

    // Divide memória compartilhada matrizes A, B, e C
    matriz_a = shared_memory;
    matriz_b = shared_memory + TAMANHO_MATRIZ;
    matriz_c = shared_memory + 2 * TAMANHO_MATRIZ;

    // Inicializa matrizes A e B
    memcpy(matriz_a, valores_linhaA, TAMANHO_MATRIZ * sizeof(int));
    memcpy(matriz_b, valores_linhaB, TAMANHO_MATRIZ * sizeof(int));

    // Cria um processo para cada linha
    for (int i = 0; i < N_LINHAS; i++) {
        int pid = fork();
            
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Processo filho
            soma_linha(i, matriz_a, matriz_b, matriz_c);
            exit(EXIT_SUCCESS);
        }
    }

    // Espera todos os processos filhos terminarem
    for (int i = 0; i < N_LINHAS; i++) {
        wait(NULL);
    }

    // Exibe a matriz C (resultado)
    printf("Matriz C (Resultado):\n");
    for (int i = 0; i < N_LINHAS; i++) {
        for (int j = 0; j < N_COLUNAS; j++) {
            printf("%d ", *(matriz_c + i * N_COLUNAS + j));
        }
        printf("\n");
    }

    // Desanexa o segmento de memória compartilhada
    if (shmdt(shared_memory) == -1) {
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

void soma_linha(int linha, int *matriz_a, int *matriz_b, int *matriz_c) {
    for (int i = 0; i < N_COLUNAS; i++) {
        *(matriz_c + linha * N_COLUNAS + i) = *(matriz_a + linha * N_COLUNAS + i) + *(matriz_b + linha * N_COLUNAS + i);
    }
}
