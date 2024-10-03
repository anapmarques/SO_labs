// Ana Luiza e Marcos Turo

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
   int pid, status;
   int num = 1;
   printf("antes do fork: %d\n", num);
   pid = fork();
   if (pid!=0) { // Pai
       waitpid(-1, &status, 0);
       printf("apos exit: %d\n",num);
   }
   else { //Filho
       num = 5;
       printf("print no filho: %d\n",num);
       exit(3);
   }
   return 0;
}

