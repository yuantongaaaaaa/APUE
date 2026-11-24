//
// Created by cliondev on 2026/8/18.
//
#include <stdio.h>
#include "memory.h"
#include "threadpool.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "ctx_switch.h"

#define STACK_SIZE (1024 * 1024)
static ctx_t sched_ctx;
static ctx_t coro_ctx;
static char *coro_stack;


void memory_test() {
    //init
    yinit(1024*1024,64);

    //malloc
    int *a = (int*)ymalloc();
    int *b = (int*)ymalloc();
    *a = 1;
    *b = 2;
    printf("a=%d,b=%d \n",*a,*b);

    //free
    yfree(a);

    //destory
    ydestory();
}

void my_task(void *arg) {
    int id = *(int *)arg;
    printf("Thread %lu is executing task %d\n", pthread_self(), id);
    usleep(500000); // 模拟耗时操作
}

void threadpool_test() {
    // 1. 创建线程池（4个线程，模拟 corePoolSize=4）
    ThreadPool *pool = pool_create(4);
    printf("ThreadPool created with 4 threads.\n");

    // 2. 提交 10 个任务
    int *ids = (int *)malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; i++) {
        ids[i] = i + 1;
        pool_add_task(pool, my_task, &ids[i]);
        printf("Task %d submitted.\n", i + 1);
    }

    free(ids);
}


void coro_func(void *arg)
{
    int id = *(int*)arg;
    for(int i = 0; i < 3; i++)
    {
        printf("【协程 %d】run %d\n", id, i);
        ctx_switch(&coro_ctx, &sched_ctx); // yield：切回调度器
    }
    printf("【协程 %d】exit\n", id);
}



void coroutine_test() {
    coro_stack = malloc(STACK_SIZE);
    int arg = 100;

    // 初始化协程上下文
    ctx_make(&coro_ctx, coro_stack, STACK_SIZE, coro_func, &arg);

    printf("main: start\n");

    // 第一次切换：保存调度器上下文，进入协程
    ctx_switch(&sched_ctx, &coro_ctx);
    printf("main: back 1\n");

    ctx_switch(&sched_ctx, &coro_ctx);
    printf("main: back 2\n");

    ctx_switch(&sched_ctx, &coro_ctx);
    printf("main: back 3\n");

    free(coro_stack);
    printf("main done\n");
}

int main() {
    //memory_test();
    //threadpool_test();

    coroutine_test();

    return 0;
}