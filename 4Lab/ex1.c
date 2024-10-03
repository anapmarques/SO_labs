// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
    int nDadosW, nDadosR, pid; // quantidade de dados transmitidos/recebidos
    int fd[2]; // descritor dos pipes
    const char textoW[] = "oi pai";
    char textoR[sizeof textoW];

    if (pipe(fd) < 0) {
        puts ("Erro ao abrir os pipes");
        exit (-1);
    }

    if ((pid = fork()) < 0) {
        perror("Erro no fork");
        exit(-1);
    }
    if (pid == 0) { // filho escreve
        close(fd[0]);
        nDadosW = write(fd[1], textoW, strlen(textoW)+1);
        printf("%d dados escritos: %s\n", nDadosW, textoW);
    }
    else { // pai le
        close(fd[1]);
        wait(NULL);
        nDadosR = read(fd[0], textoR, sizeof textoR);
        printf("%d dados lidos: %s\n", nDadosR, textoR);
    }

    close(fd[0]);
    close(fd[1]);
    return 0;
}
