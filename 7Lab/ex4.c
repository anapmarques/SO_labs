#include <stdio.h>
#include <sys/sem.h>

#define SEM_PROD 5678
#define SEM_CONS 5679

int main() {
    int sem_prod = semget(SEM_PROD, 1, 0666 | IPC_CREAT);
    int sem_cons = semget(SEM_CONS, 1, 0666 | IPC_CREAT);

    if (sem_prod < 0 || sem_cons < 0) {
        perror("Erro ao criar semáforos");
        return 1;
    }

    semctl(sem_prod, 0, SETVAL, 1);  // Produtor começa com 1 (pode escrever)
    semctl(sem_cons, 0, SETVAL, 0);  // Consumidor começa com 0 (espera)

    printf("Semáforos inicializados.\n");
    return 0;
}
