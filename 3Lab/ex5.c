// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handle_sigfpe(int sig){
    printf("Erro de ponto flutuante capturado (sinal %d)\n", sig);
    exit(1);
}

int main (int argc, char* argv[]){
    if (argc != 3){
        printf("parametros errrados\n");
        exit(1);
    }

    if (signal(SIGFPE, handle_sigfpe) == SIG_ERR) {
        printf("Erro ao configurar o manipulador de sinal\n");
        exit(1);
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    printf("Soma: %d\n", num1 + num2);
    printf("Subtração: %d\n", num1 - num2);
    printf("Multiplicação: %d\n", num1 * num2);
    printf("Divisão: %d\n", num1/num2);

    return 0;
}