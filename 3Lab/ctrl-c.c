// Ana Luiza e Marcos Turo

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void ctrlCHandler(int sinal);
void quitHandler(int sinal);

int main (void){
   void (*p)(int);

   p = signal(SIGINT, ctrlCHandler);
   printf("Endereco do manipulador anterior %p\n", p);

   p = signal(SIGQUIT, quitHandler);
   printf("Endereco do manipulador anterior %p\n", p);

   puts ("Ctrl-C desabilitado. Use Ctrl-\\ para terminar");

   while(1) {
      sleep(1);
   }

   return 0;
}

void ctrlCHandler(int sinal){
   printf("Você pressionou Ctrl-C (%d) \n", sinal);
}

void quitHandler(int sinal){
   printf("Terminando o processo...\n");
   exit(0);
}