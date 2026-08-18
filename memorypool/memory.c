//
// Created by cliondev on 2026/8/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


void yinit(int size,int blocksize) {


    pool_size = size;//global varibale assignment
    block_size = blocksize;//global varibale assignment

    //1.malloc
    pool_start = (int*)malloc(size * sizeof(int));
    if (pool_start == NULL) {
        printf("mempool yinit init: malloc failed \n");
        exit(1);
    }

    //2.split into block
    size_t num_block = pool_size / block_size;
    int* current = pool_start;
    free_list = (struct yblock*)current;

    //tranvers all block,construct linklist
    for (size_t i = 0; i < num_block ; i++) {
        yblock* b = (yblock*)current;
        b->next = (yblock*)(current+blocksize);//next block
        current += blocksize;
    }

    //last item in linklist
    yblock* last = (yblock*)current;
    last->next=NULL;

    printf("mempool yinit init ok \n");

}


void* ymalloc() {
    if (free_list == NULL) {
        printf("mempool_alloc: out of memory\n");
    }

    //linklist
    yblock* head = free_list;
    free_list = head->next;

    //return actual address
    return (void*)head;
}


void yfree(int *arr) {

    if (arr == NULL) return;

    //freeblock insert into linklist's head
    yblock* yblock1 = (yblock*)arr;
    yblock1->next = free_list;
    free_list = yblock1;

    printf("free \n");


}

void ydestory() {
    if (pool_start) {
        free(pool_start);
        pool_start = NULL;
        free_list = NULL;
    }
}















