#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

char *messages[3] = {NULL, NULL, NULL};
pthread_mutex_t mutexes[3];
pthread_cond_t conds[3];

void *messenger(void *p) {
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++) {
        long dest = (tid + 1) % 3;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        pthread_mutex_lock(&mutexes[dest]);
        messages[dest] = msg;
        pthread_cond_signal(&conds[dest]);
        pthread_mutex_unlock(&mutexes[dest]);

        pthread_mutex_lock(&mutexes[tid]);
        while (messages[tid] == NULL) {
            pthread_cond_wait(&conds[tid], &mutexes[tid]);
        }
        printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
        free(messages[tid]);
        messages[tid] = NULL;
        pthread_mutex_unlock(&mutexes[tid]);
    }
    return NULL;
}

int main() {
    pthread_t thrID1, thrID2, thrID3;

    for (int i = 0; i < 3; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
        pthread_cond_init(&conds[i], NULL);
    }

    pthread_create(&thrID1, NULL, messenger, (void *)0);
    pthread_create(&thrID2, NULL, messenger, (void *)1);
    pthread_create(&thrID3, NULL, messenger, (void *)2);

    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);
    pthread_join(thrID3, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_mutex_destroy(&mutexes[i]);
        pthread_cond_destroy(&conds[i]);
    }

    return 0;
}
