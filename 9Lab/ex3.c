#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

long long sum_file_sizes(const char *dir_path) {
    struct dirent *entry;
    struct stat file_stat;
    long long total_size = 0;
    DIR *dir = opendir(dir_path);

    if (!dir) {
        perror("Erro ao abrir o diretório");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &file_stat) == -1) {
            perror("Erro ao obter informações do arquivo");
            continue;
        }
        if (S_ISDIR(file_stat.st_mode)) {
            total_size += sum_file_sizes(path);
        } else {
            total_size += file_stat.st_size;
        }
    }
    closedir(dir);
    return total_size;
}

int main() {
    const char *start_dir = ".";
    long long total_size = sum_file_sizes(start_dir);
    printf("Tamanho total dos arquivos: %lld bytes\n", total_size);
    return 0;
}