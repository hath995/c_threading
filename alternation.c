#include <pthread.h>
#include <stdio.h>

struct tdata {
  int count;
  int turn;
  int thread;
  pthread_mutex_t lock;
  int thread_count;
};

void *countUp(void *param) {
  struct tdata* datas = (struct tdata*)param;
  pthread_mutex_lock(&datas->lock);
  int thread_id = datas->thread;
  datas->thread++;
  pthread_mutex_unlock(&datas->lock);

  while(datas->count < 20) {
    pthread_mutex_lock(&datas->lock);
    if(datas->turn == thread_id && datas->count < 20) {
      printf("thread %d: %d\n",thread_id, datas->count);
      datas->turn = (datas->turn+1) % datas->thread_count; 
      datas->count++;
    }
    pthread_mutex_unlock(&datas->lock);
  }
}

int main(int argc, char* argv[]) {
  pthread_t threads[2];
  printf("starting up\n");
  struct tdata datums = {0,0,0,PTHREAD_MUTEX_INITIALIZER,3};
  int i;
  for(i = 0; i < datums.thread_count; i++) {
    int ec = pthread_create(&threads[i], NULL, countUp, (void *)&datums);
    if (ec){
      printf("ERROR; return code from pthread_create() is %d\n", ec);
    }
  }

  pthread_exit(NULL);
}
