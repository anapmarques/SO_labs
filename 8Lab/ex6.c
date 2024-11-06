#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *filepath = "ex6.txt";
    const char *conteudo_adicional = "Nao sei o que nao sei que la\n";
    size_t tam_conteudo = strlen(conteudo_adicional);
    
    int fd = open(filepath, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Obtenha o tamanho atual do arquivo
    off_t tam_original = lseek(fd, 0, SEEK_END);
    if (tam_original == -1) {
        perror("Erro ao obter o tamanho do arquivo");
        close(fd);
        return 1;
    }

    // Calcule o novo tamanho
    off_t tam_novo = tam_original + tam_conteudo;

    // Ajuste o tamanho do arquivo com ftruncate
    if (ftruncate(fd, tam_novo) == -1) {
        perror("Erro ao ajustar o tamanho do arquivo");
        close(fd);
        return 1;
    }

    // Mapear o arquivo na memória com o tamanho original (ou 1 byte se estiver vazio)
    size_t map_size = (tam_original > 0) ? tam_original : 1;
    char *mapeamento = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapeamento == MAP_FAILED) {
        perror("Erro no mapeamento inicial do arquivo");
        close(fd);
        return 1;
    }

    // Usar mremap para expandir o mapeamento para o novo tamanho
    mapeamento = mremap(mapeamento, map_size, tam_novo, MREMAP_MAYMOVE);
    if (mapeamento == MAP_FAILED) {
        perror("Erro ao expandir o mapeamento com mremap");
        close(fd);
        return 1;
    }

    // Copiar o conteúdo adicional para a nova área do mapeamento
    memcpy(mapeamento + tam_original, conteudo_adicional, tam_conteudo);

    // Sincronizar o mapeamento com o arquivo
    if (msync(mapeamento, tam_novo, MS_SYNC) == -1) {
        perror("Erro ao sincronizar o mapeamento");
    }

    // Libere o mapeamento
    if (munmap(mapeamento, tam_novo) == -1) {
        perror("Erro ao liberar o mapeamento");
    }

    close(fd);
    return 0;
}
