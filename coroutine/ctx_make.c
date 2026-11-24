//
// Created by cliondev on 2026/9/1.
//
#include "ctx_switch.h"
#include <stddef.h>
#include <stdint.h>

extern void ctx_trampoline(void);

/**
*1.`ctx_switch` 的`ret`：当第一次切换到新上下文，`rsp`指向我们布置好的栈，`ret`就会弹出`ctx_trampoline`地址，rip 跳转到跳板。
2. 跳板`ctx_trampoline`再把`entry`和`arg`取出来，调用真正的协程函数。
3. 协程函数执行 return，跑到`hlt`停机，代表协程结束。
 *
 * @param ctx
 * @param stack_base
 * @param stack_size
 * @param entry
 * @param arg
 */
void ctx_make(ctx_t *ctx, void *stack_base, size_t stack_size,
              void (*entry)(void *arg), void *arg)
{
    // 栈顶：从栈的最高地址开始，向下生长
    uint64_t *sp = (uint64_t *)((char*)stack_base + stack_size);

    /*
     * 布置栈帧，供 ctx_trampoline 使用
     * 栈布局（从低地址到高地址）：
     * [ctx_trampoline]  <- rsp 指向这里，ctx_switch 的 ret 弹出它跳到跳板
     * [arg]             <- 跳板 pop rdi 取参数
     * [entry]           <- 跳板 pop rax 取入口函数
     */
    *(--sp) = (uint64_t)entry;            // 最高地址，最后被跳板 pop rax 取出
    *(--sp) = (uint64_t)arg;              // 跳板第一个 pop 出来作为参数
    *(--sp) = (uint64_t)ctx_trampoline;   // ret 之后跳到跳板函数

    // 初始化上下文rsp指向布置好的栈顶
    ctx->rsp = (uint64_t)sp;

    // 其余寄存器初始化为0；第一次切换过去，跳板函数会接管
    ctx->rbx = 0;
    ctx->rbp = 0;
    ctx->r12 = 0;
    ctx->r13 = 0;
    ctx->r14 = 0;
    ctx->r15 = 0;
}