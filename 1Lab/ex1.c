// Ana Luiza e Marcos Turo

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
   int mypid, pid, status;
   pid = fork();
   if (pid!=0) { // Pai
       mypid =  getpid();
       printf("pai: %d\n", mypid);
       waitpid(-1, &status, 0);
   }
   else { //Filho
       mypid = getpid();
       printf("filho: %d\n", mypid);
       exit(3);
   }
   return 0;
}

