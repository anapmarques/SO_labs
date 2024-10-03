#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXFILA 8
#define NUM_ITEMS 64
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2

int buffer[MAXFILA];
int cont = 0;
int in = 0, out = 0;
int item_counter = 1;

pthread_mutex_t mutex;
pthread_cond_t cond_prod, cond_cons;

void* producer(void* arg) {
    int id = *(int*)arg; 
    while(1) {
        sleep(1);
    
        pthread_mutex_lock(&mutex);

        if (item_counter > NUM_ITEMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (cont == MAXFILA) {
            pthread_cond_wait(&cond_prod, &mutex);
        }

        buffer[in] = item_counter;
        printf("Produtor %d produziu: %d\n", id, buffer[in]);
        item_counter++;
        in = (in + 1) % MAXFILA;
        cont++;

        pthread_cond_signal(&cond_cons);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while(1) {
        sleep(2);
    
        pthread_mutex_lock(&mutex);

        while (cont == 0) {
            if (item_counter > NUM_ITEMS) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            pthread_cond_wait(&cond_cons, &mutex);
        }

        int item = buffer[out];
        printf("Consumidor %d consumiu: %d\n", id, item);
        out = (out + 1) % MAXFILA;
        cont--;

        pthread_cond_signal(&cond_prod);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t produtores[NUM_PRODUTORES], consumidores[NUM_CONSUMIDORES];
    int prod_ids[NUM_PRODUTORES], cons_ids[NUM_CONSUMIDORES];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_prod, NULL);
    pthread_cond_init(&cond_cons, NULL);

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        prod_ids[i] = i;
        pthread_create(&produtores[i], NULL, producer, &prod_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        cons_ids[i] = i;
        pthread_create(&consumidores[i], NULL, consumer, &cons_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_prod);
    pthread_cond_destroy(&cond_cons);

    return 0;
}