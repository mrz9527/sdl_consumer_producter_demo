#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "SDL.h"
#include "Queue.h"

#define Print_Red "\033[0;31m"
#define Print_Green "\033[0;32m"
#define Print_Blue "\033[0;34m"
#define Print_Black "\033[0;30m"

using IntDeque = Queue<int>;

IntDeque iq(10);
SDL_mutex* mutex = nullptr;
SDL_cond* empty_cond = nullptr;
SDL_cond* full_cond = nullptr;

int consumer(void *arg)
{
    int threadIndex = (int)(long)arg;
    int delay_time = 600 + threadIndex * 100;
    while(true) {
       int val;
       // 从队列中取出数据
        SDL_LockMutex(mutex);
        while(iq.Empty()) { // 队列为空
            printf(Print_Red"consumer thread#%d 队列为空，阻塞中...\n", threadIndex);
            SDL_CondWait(empty_cond, mutex);
        }
        val = iq.Pop();
        SDL_UnlockMutex(mutex);

        // 通知生产者
        SDL_CondBroadcast(full_cond);

        // 执行任务
        printf(Print_Blue"consumer thread#%d: %d\n", threadIndex, val);
        SDL_Delay(delay_time);
   }
    return (0);
}

int producter(void *arg)
{
    int threadIndex = (int)(long)arg;
    int delay_time = 300 + threadIndex * 100;

    while(true) {
        // 生产数据
        SDL_Delay(delay_time);
        int val = rand() % 100000;

        // 添加数据
        SDL_LockMutex(mutex);
        while(iq.Full()) { // 队列满
            printf(Print_Red"producter thread#%d 队列已满，阻塞中...\n", threadIndex);
            SDL_CondWait(full_cond, mutex);
        }
        printf(Print_Green"producter thread#%d: %d\n", threadIndex, val);
        iq.Push(val);
        SDL_UnlockMutex(mutex);

        // 通知消费者
        SDL_CondBroadcast(empty_cond);
    }
    return (0);
}

/* 处理SIGINT信号，退出程序
 * */
void process_exit(int sig)
{
    printf(Print_Black"接收信号:%d\n", sig);

    // 创建锁和条件变量
    SDL_DestroyMutex(mutex);
    SDL_DestroyCond(empty_cond);
    SDL_DestroyCond(full_cond);

    exit(0);
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    // 设置信号
    for( int i = 1; i< 65; ++i) {
        signal(i, SIG_IGN);
    }
    signal(SIGINT, process_exit);

    // 创建锁和条件变量
    mutex = SDL_CreateMutex();
    empty_cond = SDL_CreateCond();
    full_cond = SDL_CreateCond();

    int producters = 2;
    int consumers = 3;

    // 创建生产者线程
    for (int i = 0; i < producters; ++i) {
        SDL_Thread *producter_thread = SDL_CreateThread(producter, "producter", (void *) i);
        SDL_DetachThread(producter_thread);
    }

    // 创建消费者线程
    for(int j = 0; j < consumers; ++j) {
        SDL_Thread *consumer_thread = SDL_CreateThread(consumer, "consumer", (void *) j);
        SDL_DetachThread(consumer_thread);
    }

    while(1) {
        SDL_Delay(10);
    }

    return (0);
}