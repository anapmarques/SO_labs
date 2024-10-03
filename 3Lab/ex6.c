// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile time_t start_time;
volatile int call_in_progress = 0;

void iniciar_chamada(int sig) {
    if (!call_in_progress) {
        start_time = time(NULL); // Marca o tempo de início da chamada
        call_in_progress = 1;
        printf("Chamada iniciada.\n");
    } else {
        printf("Chamada já em andamento.\n");
    }
}

void encerrar_chamada(int sig) {
    if (call_in_progress) {
        time_t end_time = time(NULL); // Marca o tempo de término da chamada
        time_t duration = end_time - start_time; // Calcula a duração em segundos
        double custo = 0.0;

        if (duration <= 60) {
            custo = duration * 0.02; // 2 centavos por segundo para até 1 minuto
        } else {
            custo = (60 * 0.02) + ((duration - 60) * 0.01); // 1 centavo por segundo a partir do 2º minuto
        }

        printf("Chamada encerrada.\n");
        printf("Duração: %ld segundos\n", duration);
        printf("Custo da ligação: R$%.2f\n", custo);
        call_in_progress = 0;
    } else {
        printf("Nenhuma chamada em andamento.\n");
    }
}

int main() {
    // Configura os sinais para iniciar e encerrar a chamada
    signal(SIGUSR1, iniciar_chamada);
    signal(SIGUSR2, encerrar_chamada);

    // Executa o programa em segundo plano
    printf("Programa em execução em background. PID: %d\n", getpid());
    while (1) {
        pause(); // Aguarda por sinais
    }

    return 0;
}