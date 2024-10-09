#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define FIFO_REQUEST "fifo_request"
#define FIFO_RESPONSE "fifo_response"
#define BUFFER_SIZE 100

void convert_to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

int main(void) {
    // Criação das FIFOs
    mkfifo(FIFO_REQUEST, 0666);
    mkfifo(FIFO_RESPONSE, 0666);

    char buffer[BUFFER_SIZE];

    printf("Servidor em execução...\n");

    while (1) {
        // Abrindo a FIFO para receber requisições
        int req_fd = open(FIFO_REQUEST, O_RDONLY);
        int res_fd = open(FIFO_RESPONSE, O_WRONLY);

        // Ler a string enviada pelo cliente
        read(req_fd, buffer, sizeof(buffer));

        // Converter para maiúsculas
        convert_to_uppercase(buffer);

        // Enviar a resposta de volta ao cliente
        write(res_fd, buffer, strlen(buffer) + 1);

        // Fechar os descritores
        close(req_fd);
        close(res_fd);
    }

    return 0;
}
