#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *threadA(void *p){
  for(int i=0; i<10; i++){
    printf("Thread ID %ld: i=%d\n", pthread_self(), i);
    usleep(1000);
  }
}
void *threadB(void *p){
  for(int i=0; i<10; i++){
    printf("Thread ID %ld: i=%d\n", pthread_self(), i);
    usleep(1000);
  }
}
void main(){
  pthread_t thrID1, thrID2;
  pthread_create(&thrID1, NULL, threadA, NULL);
  pthread_create(&thrID2, NULL, threadB, NULL);
  pthread_join(thrID1, NULL);
  pthread_join(thrID2, NULL);
}


