#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int buffer[5];
int producers_num[3] = {1, 2, 3};
int consumers_num[4] = {1, 2, 3, 4};

void show_buffer(){
    for (int i = 0; i < 5; i++)
    {
        printf("%d", buffer[i]);
    }
    printf("\n");
}

// 生产者行为
void *p_do(void *arg) {
    int num = *(int*)arg;// 生产者编号
    while (1)
    {
        // 等待缓冲池为空且可写入
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // 读取文件内容
        FILE *fp;
        fp = fopen("1.txt", "r");
        if (fp == NULL)
        {
            printf("open file error.\n");
        }
        // 查找值为0的缓冲区，并写入
        for (int i = 0; i < 5; i++)
        {
            if (buffer[i] == 0) {
                fscanf(fp, "%d", &buffer[i]);
                printf("p%d put 1 into buffer[%d]\n", num, i);
                show_buffer();
                break;
            }
        }

        // 释放资源
        fclose(fp);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

// 消费者行为
void *c_do(void *arg) {
    int num = *(int*)arg;// 消费者编号
    while (1)
    {
        // 等待缓冲池不为空且可读取
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // 查找值为1的缓冲区，并读取
        for (int i = 0; i < 5; i++)
        {
            if (buffer[i] == 1) {
                buffer[i] = 0;
                printf("c%d get 1 into buffer[%d]\n", num, i);
                show_buffer();
                break;
            }
        }

        // 释放资源
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
}

int main()
{
    // 初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);

    // 创建3个生产者进程
    pthread_t producers[3];
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&producers[i], NULL, (void *)p_do, &producers_num[i]);
    }
    // 创建4个消费者进程
    pthread_t consumers[4];
    for (int i = 0; i < 4; i++)
    {
        pthread_create(&consumers[i], NULL, (void *)c_do, &consumers_num[i]);
    }

    // 执行进程
    for (int i = 0; i < 3; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    // 释放资源
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
