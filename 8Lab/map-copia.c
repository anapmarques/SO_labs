#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

void get_time_diff(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }
    printf("%ld segundos e %ld microssegundos.\n", seconds, microseconds);
}

int main(int argc, char *argv[]) {
    struct stat sb;
    off_t len;
    char *p;
    int fd;
    struct timeval startMM, endMM, startES, endES;

    if (argc < 2) { 
        fprintf (stderr, "usage: %s <file>\n", argv[0]); 
        exit(1);
    }

    // tamanho da página do sistema
    int page_size = sysconf(_SC_PAGESIZE);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) { 
        perror("open"); 
        exit(1); 
    } 

    if (fstat(fd, &sb) == -1) { 
        perror("fstat"); 
        exit(1); 
    }

    if (!S_ISREG(sb.st_mode)) { 
        fprintf(stderr, "%s is not a file\n", argv[1]); 
        exit(1);
    }

    // Calcula o número de páginas necessárias para o arquivo
    int num_pages = (sb.st_size + page_size - 1) / page_size;

    // Mapeia o arquivo em memória
    gettimeofday(&startMM, NULL);

    p = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { 
        perror("mmap"); 
        exit(1); 
    }

    if (close(fd) == -1) { 
        perror("close"); 
        exit(1); 
    }

    for (len = 0; len < sb.st_size; len++) 
        putchar(p[len]);

    gettimeofday(&endMM, NULL);

    if (munmap(p, sb.st_size) == -1) {
        perror("munmap");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Leitura usando E/S tradicional
    gettimeofday(&startES, NULL);

    char buffer[page_size];
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, len, stdout);
    }

    gettimeofday(&endES, NULL);

    printf("O arquivo ocupa %d páginas de memória.\n", num_pages);
    printf("\nTempo decorrido ao escrever arquivo mapeado: ");
    get_time_diff(&startMM, &endMM);
    printf("\nTempo decorrido ao escrever arquivo usando E/S: ");
    get_time_diff(&startES, &endES);

    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    return 0;
}
