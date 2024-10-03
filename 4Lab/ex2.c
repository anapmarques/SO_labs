// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
    int outputFile, inputFile;

    if ((inputFile = open("arq.txt", O_RDONLY)) == -1) {
        perror("Error input");
        return -1;
    }

    if ((outputFile = open("arq_out.txt",O_RDWR|O_CREAT|O_TRUNC,0666)) == -1) {
        perror("Error output");
        return -1;
    }

    close(0);
    if (dup2(inputFile, STDIN_FILENO) == -1) {
        perror("Error dup");
        return -1;
    }
    close(inputFile);

    if(dup2(outputFile, STDOUT_FILENO) == -1) {
        perror("Error dup2");
        return -1;
    }
    close(outputFile);

    execlp("cat", "cat", NULL);

    return 0;
}


