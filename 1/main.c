#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t chopsticks [5];
sem_t max_num;// 没拿筷子的哲学家数, 防死锁
int p_num[] = {0, 1, 2, 3, 4};// 哲学家编号

void *p_do(void* arg){
    int i = *(int *)arg; // 哲学家编号

    printf("p%d is thinking.\n", i);
    sleep(2);

    sem_wait(&max_num);
    sem_wait(&chopsticks[i]);
    printf("p%d take No.%d chopstick.\n", i, i);
    sem_wait(&chopsticks[(i+1)%5]);
    printf("p%d take No.%d chopstick.\n", i, (i+1)%5);
    printf("p%d eat.\n", i);
    sleep(1);

    sem_post(&chopsticks[i]);
    printf("p%d drop No.%d chopstick.\n", i, i);
    sem_post(&chopsticks[(i+1)%5]);
    printf("p%d drop No.%d chopstick.\n", i, (i+1)%5);
    sem_post(&max_num);
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 31; i++)
    {
        printf("####################### %d #######################\n", i);
        pthread_t pids[5]; // 创建5个线程

        // 信号量初始化
        for (int j = 0; j < 5; j++)
        {
            sem_init(&chopsticks[j], 0, 1);
        }
        sem_init(&max_num, 0, 4);

        // 创建线程
        for (int j = 0; j < 5; j++)
        {
            pthread_create(&pids[j], NULL, (void *)p_do, &p_num[j]);
        }

        // 线程调用
        for (int j = 0; j < 5; j++)
        {
            pthread_join(pids[j], NULL);
        }

        // 销毁资源
        for (int j = 0; j < 5; j++)
        {
            sem_destroy(&chopsticks[j]);
        }
        sem_destroy(&max_num);
    }

    return 0;
}
