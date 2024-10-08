#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>


int main (void) {
    if (mkfifo("minhaFifo", S_IRUSR | S_IWUSR) == 0) {
        puts("FIFO criada com sucesso");
    }

    int fifo;
    char c[20];
    if ((fifo = open("minhaFifo", O_WRONLY)) < 0){
        puts ("Erro ao abrir a FIFO para escrita"); return -1;
    }
    while (1){
        if(fgets(c, 20, stdin) == NULL){
            break;
        }
        write(fifo, &c, sizeof(c));
    }

    
    close (fifo);
    return 0;
}
