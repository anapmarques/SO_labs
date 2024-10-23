#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SEM_KEY 5678

// Funções auxiliares para manipular semáforos
void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0}; // P (wait)
    semop(sem_id, &sb, 1);
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};  // V (signal)
    semop(sem_id, &sb, 1);
}

int main() {
    int shm_id = shmget(SHM_KEY, sizeof(int), 0666 | IPC_CREAT);
    int sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    
    if (shm_id < 0 || sem_id < 0) {
        perror("Erro ao criar memória ou semáforo");
        exit(1);
    }

    int *var = (int *) shmat(shm_id, NULL, 0);
    if (var == (int *) -1) {
        perror("Erro ao associar memória compartilhada");
        exit(1);
    }

    // Somar 5 à variável com controle de semáforo
    sem_wait(sem_id);
    (*var) += 5;
    printf("Valor após soma de 5: %d\n", *var);
    sem_signal(sem_id);

    shmdt(var);  // Desassociar a memória
    return 0;
}
