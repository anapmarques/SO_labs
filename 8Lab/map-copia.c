#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

#define PAGE_SIZE 4096  // Tamanho da página de memória em bytes (típico para sistemas x86-64)

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
    struct timeval start, end;

    if (argc < 2) { 
        fprintf (stderr, "usage: %s <file>\n", argv[0]); 
        exit(1);
    }

    fd = open (argv[1], O_RDONLY);

    if (fd == -1) { 
        perror ("open"); 
        exit(1); 
    } 

    if (fstat(fd, &sb)== -1) { 
        perror ("fstat"); 
        exit(1); 
    }

    if (!S_ISREG (sb.st_mode)) { 
        fprintf (stderr, "%s is not a file\n", argv[1]); 
        exit(1);
    }

    // Verificar quantas páginas de memória o arquivo ocupa
    int num_pages = (sb.st_size + PAGE_SIZE - 1) / PAGE_SIZE; // arredondado para cima
    printf("O arquivo ocupa %d páginas de memória.\n", num_pages);

    // Escrever arquivo mapeado usando mmap
    gettimeofday(&start, NULL);

    p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED) { 
        perror ("mmap"); 
        exit(1); 
    }

    if (close (fd) == -1) { 
        perror ("close"); 
        exit(1); 
    }

    for (len = 0; len < sb.st_size; len++) 
        putchar (p[len]);
    

    gettimeofday(&end, NULL);
    printf("\nTempo decorrido ao escrever arquivo mapeado: ");
    get_time_diff(&start, &end);

    // Liberação do mapeamento de memória
    if (munmap(p, sb.st_size) == -1) {
        perror("munmap");
        exit(1);
    }

    // Escrever arquivo utilizando E/S tradicional
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    gettimeofday(&start, NULL);

    char buffer[PAGE_SIZE];
    while ((len = read(fd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, len, stdout);
    }

    gettimeofday(&end, NULL);
    printf("\nTempo decorrido ao escrever arquivo usando E/S: ");
    get_time_diff(&start, &end);

    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    return 0;
}