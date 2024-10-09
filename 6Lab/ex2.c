#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO "FifoEx2"

int main (void){
    int fpFIFO;

    if (mkfifo(FIFO, S_IRUSR | S_IWUSR) == 0) {
        puts("FIFO criada com sucesso");
    }

    if ((fpFIFO = open (FIFO, O_RDONLY | O_NONBLOCK)) < 0){
        fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
        return 1;
    }

    int pid1, pid2;

    if ((pid1 = fork()) == 0){
        if ((fpFIFO = open (FIFO, O_WRONLY | O_NONBLOCK)) < 0) {
            fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
            return 1;
        }
        char *mensagem = "melancia legal :)";

        printf("Começando a escrever...\n");
        write(fpFIFO, mensagem, strlen(mensagem) + 1);
        printf("Fim da escrita\n");
        close(fpFIFO);
        exit(0);
    }
    else if ((pid2 = fork()) == 0){
        if ((fpFIFO = open (FIFO, O_WRONLY | O_NONBLOCK)) < 0) {
            fprintf (stderr, "Erro ao abrir a FIFO %s\n", FIFO);
            return 1;
        }
        char *mensagem2 = "melancia nao legal :(";

        printf("Começando a escrever...\n");
        write(fpFIFO, mensagem2, strlen(mensagem2) + 1);
        printf("Fim da escrita\n");
        close(fpFIFO);
        exit(0);
    }
    else{
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        char ch[18];
        printf("Começando a ler...\n");
        while (read (fpFIFO, &ch, sizeof(ch)) > 0)
        printf ("%s\n", ch);
        printf("Fim da leitura\n");
    }
   

    close(fpFIFO);
    return 0;
}
