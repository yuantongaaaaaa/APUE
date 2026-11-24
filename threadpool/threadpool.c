//
// Created by cliondev on 2026/8/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <threadpool.h>


static void* worker_thread(void* arg) {
    ThreadPool *pool = (ThreadPool *)arg;

    //thread pool is working
    while (1) {
        pthread_mutex_lock(&(pool->lock));

        // 条件等待：队列为空
        while (pool->queue_head == NULL) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        // 条件等待：队列为空
        if (pool->queue_head == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        Task* task = pool->queue_head;
        pool->queue_head = task->next;
        if (pool->queue_head == NULL) {
            pool->queue_tail = NULL;
        }
        pool->queue_size--;

        pthread_mutex_unlock(&(pool->lock));

        // 执行任务（在解锁后执行，不影响其他线程提交任务）
        (*(task->function))(task->arg);

        // 释放任务内存
        free(task);
    }

}



ThreadPool* pool_create(int numthread) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if (pool == NULL) return NULL;

    pool->num_thread = numthread;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->queue_size = 0;

    pthread_mutex_init(&(pool->lock), NULL);//add lock
    pthread_cond_init(&(pool->notify), NULL);

    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * numthread);
    for (int i = 0 ; i < numthread ; i++) {
        pthread_create(&(pool->threads[i]), NULL, worker_thread, (void *)pool);
    }

    return pool;

}
void pool_add_task(ThreadPool* pool,void (*function)(void *),void *arg) {
    if (pool == NULL || function == NULL) return;

    // 创建任务节点
    Task *task = (Task *)malloc(sizeof(Task));
    task->function = function;
    task->arg = arg;
    task->next = NULL;

    pthread_mutex_lock(&(pool->lock));

    // 将任务加入队列尾部
    if (pool->queue_head == NULL) {
        pool->queue_head = task;
        pool->queue_tail = task;
    } else {
        pool->queue_tail->next = task;
        pool->queue_tail = task;
    }
    pool->queue_size++;

    // 唤醒一个等待的工作线程
    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
}
void pool_destory(ThreadPool* pool,int wait_tasks_done) {

}