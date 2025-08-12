#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 3

pthread_mutex_t barrier_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;

int barrier_count = 0;

void barrier(int num_threads) {
    pthread_mutex_lock(&barrier_mutex);
    barrier_count++;

    if (barrier_count == num_threads) {
        barrier_count = 0; 
        pthread_cond_broadcast(&barrier_cond);
    } else {
        pthread_cond_wait(&barrier_cond, &barrier_mutex);
    }

    pthread_mutex_unlock(&barrier_mutex);
}

void *worker(void *arg) {
    long tid = (long)arg;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        printf("Thread %ld is working on iteration %d\n", tid, i);
        sleep(1);
        printf("Thread %ld reached the barrier on iteration %d\n", tid, i);
        barrier(NUM_THREADS);
        printf("Thread %ld passed the barrier on iteration %d\n", tid, i);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&barrier_mutex);
    pthread_cond_destroy(&barrier_cond);

    return 0;
}
