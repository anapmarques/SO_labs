// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int fd[2], pid, pid2;

    if (pipe(fd) < 0) {
        puts ("Erro ao abrir os pipes");
        exit (-1);
    }

    if ((pid = fork()) < 0) {
        perror("Erro no fork");
        exit(-1);
    }
    if (pid == 0) {
         // Processo 1 (escreve no pipe)
        close(fd[0]);  // Fecha o lado de leitura

        // Redireciona a saída padrão (stdout) para o pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        // Executa o comando 'ps'
        execlp("ps", "ps", NULL);
        perror("Erro ao executar ps");  // Se execlp falhar
        exit(EXIT_FAILURE);
    }

    if ((pid2=fork())<0){
        perror("erro fork 2");
        exit(-1);
    }
    if (pid2 == 0){
        // Processo 2 (le no pipe)
        close(fd[1]);

        // Redireciona a entrada padrão (stdin) para o pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        // Executa o comando 'wc' para contar palavras, linhas etc.
        execlp("wc", "wc", NULL);
        perror("Erro ao executar wc");  // Se execlp falhar
        exit(EXIT_FAILURE);
    }

    return 0;
}
