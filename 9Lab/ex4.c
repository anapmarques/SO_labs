#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void list_directory(const char *dir_path, int level, long long *total_size) {
    struct dirent *entry;
    struct stat file_stat;
    DIR *dir = opendir(dir_path);

    if (!dir) {
        perror("Erro ao abrir o diretório");
        return;
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
        printf("%*s[%s]\n", level * 2, "", entry->d_name);
        if (S_ISDIR(file_stat.st_mode)) {
            list_directory(path, level + 1, total_size);
        } else {
            *total_size += file_stat.st_size;
        }
    }
    closedir(dir);
}

int main() {
    const char *start_dir = ".";
    long long total_size = 0;
    list_directory(start_dir, 0, &total_size);
    printf("Tamanho total dos arquivos: %lld bytes\n", total_size);
    return 0;
}