#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SEM_PROD 5678
#define SEM_CONS 5679
#define MAX_MSG_SIZE 100

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
    int shm_id = shmget(SHM_KEY, MAX_MSG_SIZE, 0666 | IPC_CREAT);
    int sem_prod = semget(SEM_PROD, 1, 0666 | IPC_CREAT);
    int sem_cons = semget(SEM_CONS, 1, 0666 | IPC_CREAT);

    if (shm_id < 0 || sem_prod < 0 || sem_cons < 0) {
        perror("Erro ao criar memória ou semáforo");
        exit(1);
    }

    char *shared_msg = (char *) shmat(shm_id, NULL, 0);
    if (shared_msg == (char *) -1) {
        perror("Erro ao associar memória compartilhada");
        exit(1);
    }

    while (1) {
        // Espera para poder ler (Consumidor)
        sem_wait(sem_cons);

        printf("Mensagem recebida: %s\n", shared_msg);

        // Sinaliza que a mensagem foi lida e o produtor pode escrever novamente
        sem_signal(sem_prod);
    }

    shmdt(shared_msg);  // Desassociar a memória
    return 0;
}
