#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <omp.h>

int booksAvailable = 3;

void *borrower(void *p)
{
  for (int i = 0; i <= 10; i++)
  {
    printf("Borrower %ld wants to borrow a book. ", (long)p);

    int temp;
    #pragma omp atomic read
    temp = booksAvailable;
    printf("Books available = %d\n", temp);

    #pragma omp atomic capture
    temp = booksAvailable--;
    printf("Borrower %ld gets a book. ", (long)p);

    #pragma omp atomic read
    temp = booksAvailable;
    printf("Books available = %d\n", temp);

    usleep(10000); // Reading book
    printf("Borrower %ld is returning a book. ", (long)p);

    #pragma omp atomic capture
    temp = booksAvailable++;
    #pragma omp atomic read
    temp = booksAvailable;
    printf("Books available = %d\n", temp);

    usleep(10000);
  }
  return NULL;
}


void main()
{
  pthread_t thrID1, thrID2, thrID3;
  pthread_t thrID4, thrID5, thrID6;

  pthread_create(&thrID1, NULL, borrower, (void *)1);
  pthread_create(&thrID2, NULL, borrower, (void *)2);
  pthread_create(&thrID3, NULL, borrower, (void *)3);
  pthread_create(&thrID4, NULL, borrower, (void *)4);
  pthread_create(&thrID5, NULL, borrower, (void *)5);
  pthread_create(&thrID6, NULL, borrower, (void *)6);
  pthread_join(thrID1, NULL);
  pthread_join(thrID2, NULL);
  pthread_join(thrID3, NULL);
  pthread_join(thrID4, NULL);
  pthread_join(thrID5, NULL);
  pthread_join(thrID6, NULL);
}
