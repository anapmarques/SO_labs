#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXFILA 8
#define NUM_ITEMS 64

int buffer[MAXFILA];
int in = 0, out = 0;
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t not_full, not_empty;

void *produtor (void *arg){
   for(int i = 1; i <= NUM_ITEMS; i++){
       sleep(1);
       pthread_mutex_lock(&mutex);
       while(count == MAXFILA){
           pthread_cond_wait(&not_full, &mutex);
       }

       buffer[in] = i;
       printf("Produtor produziu: %d\n", buffer[in]);
       in = (in + 1) % MAXFILA; 
       count++; 

       pthread_cond_signal(&not_empty);
      
       pthread_mutex_unlock(&mutex);
   }
   pthread_exit(NULL);
}

void *consumidor(void *arg) {
   for (int i = 1; i <= NUM_ITEMS; i++) {
       sleep(2);

       pthread_mutex_lock(&mutex);

       while(count == 0) {
           pthread_cond_wait(&not_empty, &mutex);
       }

       int item = buffer[out];
       printf("Consumidor consumiu: %d\n", item);
       out = (out + 1) % MAXFILA;
       count--;

       pthread_cond_signal(&not_full);

       pthread_mutex_unlock(&mutex);
   }
   pthread_exit(NULL);
}

int main() {
   pthread_t prod_thread, cons_thread;

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&not_full, NULL);
   pthread_cond_init(&not_empty, NULL);

   pthread_create(&prod_thread, NULL, produtor, NULL);
   pthread_create(&cons_thread, NULL, consumidor, NULL);

   pthread_join(prod_thread, NULL);
   pthread_join(cons_thread, NULL);

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&not_full);
   pthread_cond_destroy(&not_empty);

   return 0;
}