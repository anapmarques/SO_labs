#include <stdio.h>
#include <sys/sem.h>

#define SEM_KEY 5678

int main() {
    int sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (sem_id < 0) {
        perror("Erro ao criar semáforo");
        return 1;
    }

    semctl(sem_id, 0, SETVAL, 1);  // Inicializa o semáforo com valor 1
    printf("Semáforo inicializado.\n");
    return 0;
}
