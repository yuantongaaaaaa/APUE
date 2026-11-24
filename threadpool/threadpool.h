//
// Created by cliondev on 2026/8/19.
//

#ifndef APUE_THREADPOOL_H
#define APUE_THREADPOOL_H


#include <pthread.h>


//blocking queue's node
typedef struct Task {
    void (*function)(void *arg);   // 任务函数指针
    void *arg;                     // 任务参数
    struct Task* next;
} Task;

//threadpool
typedef struct {
    pthread_t* threads;//thread array
    int num_thread;//thread number

    Task* queue_head;
    Task* queue_tail;
    int queue_size;//queue size

    pthread_mutex_t lock;//lock
    pthread_cond_t notify;

} ThreadPool;

ThreadPool* pool_create(int numthread);
void pool_add_task(ThreadPool* pool,void (*function)(void *),void *arg);
void pool_destory(ThreadPool* pool,int wait_tasks_done);

#endif //APUE_THREADPOOL_H


