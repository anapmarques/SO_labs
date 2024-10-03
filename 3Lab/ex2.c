// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sigkill(int sinal){
   printf("Sinal SIGKILL recebido\n");
}

int main (){
   signal(SIGKILL, handle_sigkill);

   while(1){
     sleep(1);
   }

   return 0;
}
