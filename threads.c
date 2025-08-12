#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *threadFunction(void *p){
    for (int i = 0; i < 10; i++){
        printf("tread id %ld: i=%d\n", pthread_self(), i);
        usleep(1000);
    }
    return NULL;
}
int main(int argc, char *argv[]){
    if (argc!= 2){
        fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    if (numThreads <= 0){
        fprintf(stderr, "number of threadsshould be a positive integer.\n");
        return 1;
    }

    pthread_t *threads = (pthread_t *)malloc(numThreads *sizeof(pthread_t));
    if (threads ==NULL){
        fprintf(stderr, "failed to allocate memory for thread Ids.\n");
        return 1;
    }

    for (int i= 0; i <numThreads; i++){
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) !=0){
            fprintf(stderr, "error creating thread %d\n", i);
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i <numThreads; i++){
        pthread_join(threads[i], NULL);
    }
    free(threads);
    return 0;
}

