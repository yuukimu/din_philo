/***********************
gcc -o din_philo din_philo.c -pthread
************************/
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include "pthread.h"

#define N 5 // 哲学者の人数
int philosopher[N]; // 哲学者を表す配列
int Fork[N];  // それぞれのフォークの状態を格納(置かれている: -1, 取られている: 0から4)
pthread_mutex_t mutex;

void init();  // 初期化
int GetRandom(int min,int max);
void *func_thread(void *p);
int take(int fn, int pn);
void put(int fn, int pn);
void think(int num);
void eat(int num);

int main(int argc, char const *argv[])
{
  init();
  pthread_t pthread, pthread2, pthread3, pthread4, pthread5;
  pthread_create(&pthread, NULL, &func_thread, &philosopher[0]);
  pthread_create(&pthread2, NULL, &func_thread, &philosopher[1]);
  pthread_create(&pthread3, NULL, &func_thread, &philosopher[2]);
  pthread_create(&pthread4, NULL, &func_thread, &philosopher[3]);
  pthread_create(&pthread5, NULL, &func_thread, &philosopher[4]);
  pthread_join(pthread, NULL);
  pthread_join(pthread2, NULL);
  pthread_join(pthread3, NULL);
  pthread_join(pthread4, NULL);
  pthread_join(pthread5, NULL);
  printf("Finish!!\n");
  return 0;
}

void init(){
  int i;
  for (i = 0; i < N; ++i) {
    philosopher[i] = i;
    Fork[i] = -1;
  }
}

int GetRandom(int min,int max)
{
  srand((unsigned int)time(NULL));
  return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

void *func_thread(void *p){
  int count = 0, num = *(int*)p;
  int wait_count = 0;
  int u1, u2;
  if (num == 0){
    u1 = 0;
    u2 = 4;
  } else{
    u1 = num - 1;
    u2 = num;
  }
  printf("start %d\n", num);
  while(count < 5){
    pthread_mutex_lock(&mutex);
    if (take(u1, num) || Fork[u1] == num){
      if(take(u2, num)){
        eat(num);
        count++;
        put(u2, num);
        put(u1, num);
      } else if(wait_count == 0){
        usleep(GetRandom(1,3000000));
        wait_count++;
      } else{
        put(u1, num);
        wait_count = 0;
      }
    } else{
      think(num);
    }
    pthread_mutex_unlock(&mutex);
  }
  printf("end %d\n", num);
  return 0;
}

int take(int fn, int pn){
  if (Fork[fn] == -1){
    Fork[fn] = pn;
    return 1;
  }
  return 0;
}
void put(int fn, int pn){
  if (Fork[fn] == pn){
    Fork[fn] = -1;
  }
}

void think(int num){
  printf("philosopher %d is thinking.\n", num);
  sleep(1);
}

void eat(int num){
  sleep(1);
  printf("philosopher %d is eating.\n", num);
}