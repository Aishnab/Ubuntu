#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Struct to hold thread arguments
struct threadArgs {
    int start;
    int finish;
    int threadid;
};

// Shared variables and synchronization tools
int primesFound = 0; // Counter for total primes found
pthread_mutex_t lock; // Mutex for synchronizing access to the counter

// Thread function
void *threadMain(void *p) {
    struct threadArgs *pargs = (struct threadArgs *)p;
    int nstart = pargs->start, nfinish = pargs->finish;
    int threadid = pargs->threadid;

    for (int i = nstart; i <= nfinish; i++) {
        pthread_testcancel(); // Check if the thread has been canceled

        if (i < 2) continue; // Skip numbers less than 2
        int isPrime = 1; // Assume number is prime
        for (int c = 2; c * c <= i; c++) { // Check divisors up to sqrt(i)
            if (i % c == 0) {
                isPrime = 0;
                break;
            }
        }

        if (isPrime) {
            pthread_mutex_lock(&lock); // Lock the mutex
            if (primesFound >= 5) {
                pthread_mutex_unlock(&lock); // Unlock and exit if condition met
                pthread_exit(NULL);
            }
            primesFound++;
            printf("Thread %d found prime: %d (Total primes: %d)\n", threadid, i, primesFound);
            pthread_mutex_unlock(&lock); // Unlock the mutex

            if (primesFound >= 5) { // Cancel all threads if condition met
                pthread_exit(NULL);
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int numThreads = 0;

    printf("Enter the number of threads: ");
    scanf("%d", &numThreads);

    if (numThreads <= 0 || numThreads > 100) {
        printf("Invalid number of threads. Please enter a value between 1 and 100.\n");
        return 1;
    }

    pthread_t *thrID = malloc(numThreads * sizeof(pthread_t));
    struct threadArgs *targs = malloc(numThreads * sizeof(struct threadArgs));
    int range = 10000;
    int chunkSize = range / numThreads;

    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads
    for (int i = 0; i < numThreads; i++) {
        targs[i].start = i * chunkSize + 1;
        targs[i].finish = (i == numThreads - 1) ? range : (i + 1) * chunkSize; // Handle the last chunk
        targs[i].threadid = i + 1;
        pthread_create(&thrID[i], NULL, threadMain, &targs[i]);
    }

    // Monitor and cancel threads if condition met
    for (int i = 0; i < numThreads; i++) {
        pthread_join(thrID[i], NULL);
    }

    printf("5 prime numbers found. All threads have been canceled.\n");

    // Cleanup
    free(thrID);
    free(targs);
    pthread_mutex_destroy(&lock);

    return 0;
}
