#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS     5

typedef struct fibarg {
  int arg;
  int result;
} FIBN;

void *fib(void *arg)
{
   FIBN* self = (FIBN*)arg;
   if(self->arg <= 1) {
      self->result = 1;
   }else{
      FIBN* x = new FIBN;
      x->arg = self->arg-1;
      FIBN* y = new FIBN;
      y->arg = self->arg-2;
      pthread_t xthread;
      int return_code = pthread_create(&xthread, NULL, fib, (void*) x);
      if(return_code) {
        printf("ERROR: return code from pthread_create is %d\n", return_code);
      }
      fib(y);
      void* status;
      return_code = pthread_join(xthread, &status);
      if(return_code) {
        printf("ERROR: return code from pthread_join is %d\n", return_code);
      }
      self->result = x->result + y->result;
      delete x;
      delete y;
   }
   //printf("Hello World! It's me, thread #%ld!\n", tid);
   //pthread_exit(NULL);
   return arg;
}

int main (int argc, char *argv[])
{
   /*
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   */
   FIBN request;
   request.arg = atoi(argv[1]);
   fib((void*)&request);
   printf("%d\n",request.result);

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
