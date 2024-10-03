// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() { 
    int pid1, pid2; 
    if ((pid1 = fork()) == 0) { // Processo filho 1 
        while (1) { 
            printf("Filho 1 está executando...\n"); 
            sleep(2);
        } 
    } 
    if ((pid2 = fork()) == 0) { // Processo filho 2 
        while (1) { 
            printf("Filho 2 está executando...\n"); 
            sleep(2); 
        } 
    }
    for (int i = 0; i < 5; i++) {
        kill(pid1, SIGCONT);
        kill(pid2, SIGSTOP);
        sleep(1);

        kill(pid1, SIGSTOP);
        kill(pid2, SIGCONT);
        sleep(1);
    }

    kill(pid1, SIGKILL);
    kill(pid2, SIGKILL);

    return 0;
}