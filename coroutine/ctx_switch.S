/* ctx_switch.S  GNU gas  x86-64 System V ABI Linux */
.intel_syntax noprefix
.section .text
.global ctx_switch
.global ctx_trampoline

/*
void ctx_switch(ctx_t *old, const ctx_t *new);
rdi = old
rsi = new
*/
ctx_switch:
    /* ========== 第一步：保存当前寄存器到 old 上下文 ========== */
    mov     [rdi],      rbx
    mov     [rdi+8],    rbp
    mov     [rdi+16],   r12
    mov     [rdi+24],   r13
    mov     [rdi+32],   r14
    mov     [rdi+40],   r15
    mov     [rdi+48],   rsp

    /* ========== 第二步：从 new 上下文恢复寄存器 ========== */
    mov     rbx,      [rsi]
    mov     rbp,      [rsi+8]
    mov     r12,      [rsi+16]
    mov     r13,      [rsi+24]
    mov     r14,      [rsi+32]
    mov     r15,      [rsi+40]
    mov     rsp,      [rsi+48]

    /* ret 会从栈顶弹出返回地址到rip，完成跳转 */
    ret

/* 辅助跳板函数，不能被C直接调用 */
ctx_trampoline:
    /*
      栈布局（低 -> 高）：[arg][entry]
      ret 之后跳到这里，rsp 指向 arg
    */
    pop     rdi         /* rdi = arg，作为entry的第一个参数 */
    pop     rax         /* rax = entry 函数指针 */
    call    rax
    /* 协程入口函数return之后，直接退出，这里可以做终止处理 */
    hlt
