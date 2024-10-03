// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(void){
    int pid, pid2, pid3, fd[2]; // descritor dos pipes

    if (pipe(fd) < 0) {
        puts ("Erro ao abrir os pipes");
        exit (-1);
    }

    if ((pid=fork()) == 0){ // processo leitor
        // Fecha escrita
        close(fd[1]); 

        char buffer[BUFFER_SIZE];
        while (read(fd[0], buffer, sizeof(buffer)) > 0) {
            printf("Leitor 1: leu '%s'\n", buffer);
            sleep(2);  // Dorme 2 segundos (o dobro do tempo do escritor)
        }

        close(fd[0]);  // Fecha o lado de leitura do pipe
    }

    if ((pid2=fork()) == 0) { // processo leitor
        // Fecha escrita
        close(fd[1]); 

        char buffer[BUFFER_SIZE];
        while (read(fd[0], buffer, sizeof(buffer)) > 0) {
            printf("Leitor 2: leu '%s'\n", buffer);
            sleep(2);  // Dorme 2 segundos (o dobro do tempo do escritor)
        }

        close(fd[0]);  // Fecha o lado de leitura do pipe
    }

    if ((pid3 = fork()) == 0) { // processo escritor
        // Fecha leitura
        close(fd[0]); 

        const char* mensagens[] = {"Mensagem 1", "Mensagem 2", "Mensagem 3"};
        for (int i = 0; i < 3; i++) {
            write(fd[1], mensagens[i], strlen(mensagens[i]) + 1);  // Envia mensagem pela pipe
            printf("Escritor: enviou '%s'\n", mensagens[i]);
            sleep(1);  // Dorme 1 segundo
        }

        close(fd[1]);  // Fecha o lado de escrita do pipe
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}