#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lockBooks;
int booksAvailable = 3;

void *borrower(void *p)
{
  for (int i = 0; i <= 10; i++)
  {
    printf("Borrower %d wants to borrow a book. ", (int)p);
    printf("Books available = %d\n", booksAvailable);
    omp_set_lock(&lockBooks); // Borrowing a book
    printf("Borrower %d gets a book. ", (int)p);
    booksAvailable--;
    printf("Books available = %d\n", booksAvailable);
    usleep(10000); // reading book
    printf("Borrower %d is returning a book. ", (int)p);
    booksAvailable++;
    printf("Books available = %d\n", booksAvailable);
    omp_unset_lock(&lockBooks); // Returning a book
    usleep(10000);
  }
  return NULL;
}

void main()
{
  pthread_t thrID1, thrID2, thrID3;
  pthread_t thrID4, thrID5, thrID6;

  omp_init_lock(&lockBooks);
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
  omp_destroy_lock(&lockBooks);
}
