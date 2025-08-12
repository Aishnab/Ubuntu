#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct threadArgs {
  int start;
  int finish;
        int threadid;
};

void *threadMain(void *p){
  struct threadArgs *pargs = p;
  int i, c;
  int nstart=pargs->start, nfinish=pargs->finish;
  int  threadid = pargs->threadid;

for(i=nstart; i<=nfinish; i++){
    for(c=2; c<=i-1; c++) {
      if ( i%c==0 )
        break;
    }
    if ( c==i )
      printf("Thread %d : %d\n",threadid, i);
  }
  return 0;
}
void main(int argc, char **argv){

  int numThreads = 0;

        printf("enetr a no of theads ");
scanf("%d", &numThreads);
 int i;
  pthread_t *thrID=malloc(numThreads * sizeof(pthread_t));

struct threadArgs  *targs= malloc(numThreads * sizeof(struct threadArgs));
  if ( argc > 1 ) {
    numThreads = atoi(argv[1]);
  }
  if (numThreads > 0 && numThreads <= 100){
    int chunksize = 10000/numThreads ;
    for (i=0; i < numThreads; i++){
      targs[i].start = i * chunksize;
      targs[i].finish = (i * chunksize) + chunksize;
        targs[i].threadid = i+1;
      pthread_create(&thrID[i], NULL, threadMain, &targs[i]);
    }
    for (i=0; i < numThreads; i++){
      pthread_join(thrID[i], NULL);
    }
  }
}
