#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *filepath = "ex6.txt";
    const char *conteudo_adicional = "Adicionando mais conteúdo...\n";
    size_t tam_conteudo = strlen(conteudo_adicional);

    // Abrir o arquivo para leitura e escrita
    int fd = open(filepath, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Obter o tamanho atual do arquivo
    off_t tam_original = lseek(fd, 0, SEEK_END);
    if (tam_original == -1) {
        perror("Erro ao obter o tamanho do arquivo");
        close(fd);
        return 1;
    }

    // Mapear o arquivo para a memória
    char *mapeamento = mmap(NULL, tam_original, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapeamento == MAP_FAILED) {
        perror("Erro no mapeamento do arquivo");
        close(fd);
        return 1;
    }

    // Exibir o conteúdo original
    printf("Conteúdo original mapeado: %s\n", mapeamento);

    // Novo tamanho desejado (tamanho original + tamanho do conteúdo adicional)
    off_t tam_novo = tam_original + tam_conteudo;

    // Usando mremap para expandir o mapeamento
    mapeamento = mremap(mapeamento, tam_original, tam_novo, 0);
    if (mapeamento == MAP_FAILED) {
        perror("Erro ao redimensionar o mapeamento");
        munmap(mapeamento, tam_original); // Liberar o mapeamento original em caso de erro
        close(fd);
        return 1;
    }

    // Copiar o conteúdo adicional para o mapeamento expandido
    memcpy(mapeamento + tam_original, conteudo_adicional, tam_conteudo);

    // Sincronizar o mapeamento para o arquivo
    if (msync(mapeamento, tam_novo, MS_SYNC) == -1) {
        perror("Erro ao sincronizar o mapeamento");
    }

    // Exibir o conteúdo final
    printf("Conteúdo final mapeado: %s\n", mapeamento);

    // Liberar o mapeamento
    if (munmap(mapeamento, tam_novo) == -1) {
        perror("Erro ao liberar o mapeamento");
    }

    // Fechar o arquivo
    close(fd);

    return 0;
}
