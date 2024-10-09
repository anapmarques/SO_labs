#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_REQUEST "fifo_request"
#define FIFO_RESPONSE "fifo_response"
#define BUFFER_SIZE 100

int main(void) {
    char buffer[BUFFER_SIZE];

    // Abrindo a FIFO para escrita
    int req_fd = open(FIFO_REQUEST, O_WRONLY);
    // Abrindo a FIFO para leitura
    int res_fd = open(FIFO_RESPONSE, O_RDONLY);

    // Exemplo de mensagem para Cliente 2
    printf("Cliente 2: Digite uma string: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remover a nova linha

    // Enviar a string para o servidor
    write(req_fd, buffer, strlen(buffer) + 1);

    // Receber a resposta do servidor
    read(res_fd, buffer, sizeof(buffer));

    // Mostrar a resposta
    printf("Cliente 2: Resposta do servidor: %s\n", buffer);

    // Fechar os descritores
    close(req_fd);
    close(res_fd);

    return 0;
}
