//
// Created by cliondev on 2026/9/1.
//

#ifndef APUE_CTX_SWITCH_H
#define APUE_CTX_SWITCH_H

#include <stdlib.h>
#include <stdint.h>
// 协程上下文，只保存被调用者保存寄存器 + rsp
typedef struct {
    uint64_t rbx;
    uint64_t rbp;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rsp;
} ctx_t;

/**
 * @brief 上下文切换
 * @param old 保存当前上下文到此
 * @param new 加载此上下文运行
 */
void ctx_switch(ctx_t *old, const ctx_t *new);

/**
 * @brief 在协程栈上初始化上下文，准备执行入口函数
 * @param ctx 待初始化上下文
 * @param stack_base 栈内存起始地址(malloc出来的)
 * @param stack_size 栈大小
 * @param entry 协程入口函数 void(*entry)(void* arg)
 * @param arg 传给entry的参数
 */
void ctx_make(ctx_t *ctx, void *stack_base, size_t stack_size,
              void (*entry)(void *arg), void *arg);



#endif //APUE_CTX_SWITCH_H
