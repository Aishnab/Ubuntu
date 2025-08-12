#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 4
#define NUM_WRITERS 2

// Read-write lock structure
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t readers_ok;
    pthread_cond_t writer_ok;
    int readers;
    int writer;
} rw_lock_t;

// Initialize read-write lock
void rw_lock_init(rw_lock_t *rw) {
    pthread_mutex_init(&rw->lock, NULL);
    pthread_cond_init(&rw->readers_ok, NULL);
    pthread_cond_init(&rw->writer_ok, NULL);
    rw->readers = 0;
    rw->writer = 0;
}

// Acquire read lock
void rw_lock_acquire_read(rw_lock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    while (rw->writer) {
        pthread_cond_wait(&rw->readers_ok, &rw->lock);
    }
    rw->readers++;
    pthread_mutex_unlock(&rw->lock);
}

// Release read lock
void rw_lock_release_read(rw_lock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    rw->readers--;
    if (rw->readers == 0) {
        pthread_cond_signal(&rw->writer_ok);
    }
    pthread_mutex_unlock(&rw->lock);
}

// Acquire write lock
void rw_lock_acquire_write(rw_lock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    while (rw->writer || rw->readers > 0) {
        pthread_cond_wait(&rw->writer_ok, &rw->lock);
    }
    rw->writer = 1;
    pthread_mutex_unlock(&rw->lock);
}

// Release write lock
void rw_lock_release_write(rw_lock_t *rw) {
    pthread_mutex_lock(&rw->lock);
    rw->writer = 0;
    pthread_cond_broadcast(&rw->readers_ok);
    pthread_cond_signal(&rw->writer_ok);
    pthread_mutex_unlock(&rw->lock);
}

// Shared data
int shared_data = 0;
rw_lock_t rw_lock;

// Reader function
void* reader(void* arg) {
    int reader_id = *(int*)arg;
    while (1) {
        rw_lock_acquire_read(&rw_lock);
        printf("Reader %d is reading the shared data: %d\n", reader_id, shared_data);
        sleep(1);
        rw_lock_release_read(&rw_lock);
        sleep(1);
    }
    return NULL;
}

// Writer function
void* writer(void* arg) {
    int writer_id = *(int*)arg;
    while (1) {
        rw_lock_acquire_write(&rw_lock);
        shared_data++;
        printf("Writer %d has modified the shared data to: %d\n", writer_id, shared_data);
        sleep(2);
        rw_lock_release_write(&rw_lock);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    rw_lock_init(&rw_lock);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join threads (not strictly necessary here since the threads run indefinitely)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
