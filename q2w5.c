#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

char *messages[3] = {NULL, NULL, NULL};
atomic_int message_ready[3];

void *messenger(void *p) {
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++) {
        long dest = (tid + 1) % 3;

        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        while (atomic_load(&message_ready[dest]) != 0); // Busy-wait until destination is free
        messages[dest] = msg;
        atomic_store(&message_ready[dest], 1);
        printf("Thread %ld sent the message to Thread %ld\n", tid, dest);

        while (atomic_load(&message_ready[tid]) == 0); // Busy-wait until message is ready for this thread
        printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
        free(messages[tid]);
        messages[tid] = NULL;
        atomic_store(&message_ready[tid], 0);
    }
    return NULL;
}

int main() {
    pthread_t thrID1, thrID2, thrID3;

    for (int i = 0; i < 3; i++) {
        atomic_init(&message_ready[i], 0);
    }

    pthread_create(&thrID1, NULL, messenger, (void *)0);
    pthread_create(&thrID2, NULL, messenger, (void *)1);
    pthread_create(&thrID3, NULL, messenger, (void *)2);

    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);
    pthread_join(thrID3, NULL);

    return 0;
}
