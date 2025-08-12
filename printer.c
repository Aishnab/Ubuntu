#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_USERS 5
#define NUM_PRINTERS 2

sem_t printerSemaphore;
void *userFunction(void *arg) {
    int userId = *(int *)arg;
    printf("User %d is waiting to use a printer.\n", userId);
    sem_wait(&printerSemaphore);
    printf("User %d is using a printer.\n", userId);
    sleep(2);
    printf("User %d has finished printing.\n", userId);
    sem_post(&printerSemaphore);

    return NULL;
}

int main() {
    pthread_t users[NUM_USERS];
    int userIds[NUM_USERS];
    sem_init(&printerSemaphore, 0, NUM_PRINTERS);
    for (int i = 0; i < NUM_USERS; i++) {
        userIds[i] = i + 1;
        pthread_create(&users[i], NULL, userFunction, &userIds[i]);
    }
    for (int i = 0; i < NUM_USERS; i++) {
        pthread_join(users[i], NULL);
    }
    sem_destroy(&printerSemaphore);

    return 0;
}
