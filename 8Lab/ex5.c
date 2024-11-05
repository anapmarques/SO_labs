#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *filepath = "ex5.txt";
    const char *conteudo_adicional = "Nao sei o que nao sei que la\n";
    size_t tam_conteudo = strlen(conteudo_adicional);

    int fd = open(filepath, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // tamanho atual do arquivo 
    off_t tam_original = lseek(fd, 0, SEEK_END);
    if (tam_original == -1) {
        perror("Erro ao obter o tamanho do arquivo");
        close(fd);
        return 1;
    }

    // novo tamanho 
    off_t tam_novo = tam_original + tam_conteudo;

    // novo tamanho do arquivo com ftruncate
    if (ftruncate(fd, tam_novo) == -1) {
        perror("Erro ao ajustar o tamanho do arquivo");
        close(fd);
        return 1;
    }

    //mapear o arquivo na memória
    char *mapeamento = mmap(NULL, tam_novo, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapeamento == MAP_FAILED) {
        perror("Erro no mapeamento do arquivo");
        close(fd);
        return 1;
    }

    memcpy(mapeamento + tam_original, conteudo_adicional, tam_conteudo);

    if (msync(mapeamento, tam_novo, MS_SYNC) == -1) {
        perror("Erro ao sincronizar o mapeamento");
    }

    if (munmap(mapeamento, tam_novo) == -1) {
        perror("Erro ao liberar o mapeamento");
    }

    close(fd);
    return 0;
}
