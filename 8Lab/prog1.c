#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FILE_PATH "shared_file"
#define FILE_SIZE 256
#define MESSAGE_SIZE 100

int main() {
    int fd;
    char *mapped_mem;

    // Cria ou abre o arquivo para leitura e escrita
    fd = open(FILE_PATH, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Define o tamanho do arquivo
    ftruncate(fd, FILE_SIZE);

    // Mapeia o arquivo na memória
    mapped_mem = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_mem == MAP_FAILED) {
        perror("Erro ao mapear o arquivo");
        close(fd);
        return 1;
    }

    if (mapped_mem[0] == 0){
        // Escreve uma mensagem no arquivo mapeado
        char message[MESSAGE_SIZE];
        printf("Escreva sua mensagem: ");
        fgets(message, MESSAGE_SIZE, stdin);
        message[strlen(message) - 1] = '\0';
        strncpy(mapped_mem, message, FILE_SIZE);
        printf("prog1: Escreveu '%s' no arquivo mapeado.\n", message);
    }
    else {
        // Lê a mensagem do arquivo mapeado
        printf("prog1: Leu '%s' do arquivo mapeado.\n", mapped_mem);
        memset(mapped_mem, 0, FILE_SIZE);
    }
    

    // Desfaz o mapeamento e fecha o arquivo
    munmap(mapped_mem, FILE_SIZE);
    close(fd);

    return 0;
}
