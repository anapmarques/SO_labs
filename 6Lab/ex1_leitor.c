#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main (void) {
    int fifo;
    char ch[20];
    if ((fifo = open("minhaFifo", O_RDONLY)) < 0) {
        puts ("Erro ao abrir a FIFO para escrita"); return -1;
    }
    while (read(fifo, &ch, sizeof(ch)) > 0) puts (ch);
    printf("%s", ch);
    close (fifo);
    return 0;
}