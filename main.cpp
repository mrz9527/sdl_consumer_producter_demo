#include <iostream>
#include "SDL_thread.h"
#include "SDL_mutex.h"

int potty = 0;
int gotta_go;

int thread_func(void *data)
{
    SDL_mutex *lock = (SDL_mutex *)data;
    int times_went;

    times_went = 0;
    while ( gotta_go ) {
        SDL_mutexP(lock);    /* 锁住potty */
        ++potty;
        printf("线程%d 正在使用potty/n", SDL_ThreadID());
        if ( potty > 1 ) {
            printf("哦，有人在用potty!/n");
        }
        --potty;
        SDL_mutexV(lock);
        ++times_went;
    }
    printf("好了/n");
    return(times_went);
}

int main(int argc,char *argv[])
{
    const int progeny = 5;
    SDL_Thread *kids[progeny];
    SDL_mutex  *lock;
    int i, lots;

    /* 创建同步锁 */
    lock = SDL_CreateMutex();

    gotta_go = 1;
    for ( i=0; i<progeny; ++i ) {
        kids[i] = SDL_CreateThread(thread_func, lock);
    }

    SDL_Delay(5*1000);
    SDL_mutexP(lock);
    printf("都完成了吗？/n");
    gotta_go = 0;
    SDL_mutexV(lock);

    for ( i=0; i<progeny; ++i ) {
        SDL_WaitThread(kids[i], &lots);
        printf("线程 %d 用过potty %d 次了/n", i+1, lots);
    }
    SDL_DestroyMutex(lock);
    returtn 0;
}