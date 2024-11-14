#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#define FALSE 0
#define TRUE 1

extern int alphasort();
char pathname[MAXPATHLEN];

int file_select(const struct direct *entry) {
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return (FALSE);
    else
        return (TRUE);
}

void print_file_info(const char* filename){
    struct stat file_stat;

    if(stat(filename, &file_stat) == -1){
        perror("File error");
        return;
    }

    printf("%s, ", filename);
    printf("inode %ld, ", file_stat.st_ino);
    printf("size: %ld, ", file_stat.st_size);

    time_t current_time = time(NULL);
    double age_in_days = difftime(current_time, file_stat.st_mtime) / (60 * 60 * 24);
    printf("age: %f days.", age_in_days);
}

int main(void) {
    int count,i;
    struct direct **files;

    int file_select();

    if (getwd(pathname) == NULL ) { 
        printf("Error getting path\n"); exit(0);
    }
    printf("Current Working Directory = %s\n",pathname);
    count = scandir( pathname, &files, file_select, alphasort);

    /* If no files found, make a non-selectable menu item */
    if (count <= 0) { 
        printf("No files in this directory\n"); exit(0);
    }
    printf("Number of files = %d\n",count);

    for (i=1;i<count+1;++i) {
        print_file_info(files[i-1]->d_name);
        printf("\n"); /* flush buffer */
    }
}