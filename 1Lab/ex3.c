// Ana Luiza e Marcos Turo

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int compare(const void* a, const void* b){
   return (*(int*)a - *(int*)b);
}


int main(void){
   int pid, status;
   int vetor[10] = {0,5,3,8,6,2,4,9,1,7};
   printf("antes fork: ");
   for (int i = 0; i<10;i++){
       printf("%d ",vetor[i]);
   }
   printf("\n");
   pid = fork();
   if (pid!=0) { // Pai
       waitpid(-1, &status, 0);
       printf("depois waitpid: ");
       for (int i = 0; i<10;i++){
           printf("%d ",vetor[i]);
       }
       printf("\n");
   }
   else { //Filho
       qsort(vetor, 10, sizeof(int), compare);
       printf("vetor filho: ");
       for (int i = 0; i<10;i++){
           printf("%d ",vetor[i]);
       }
       printf("\n");
       exit(3);
   }
   return 0;
}

