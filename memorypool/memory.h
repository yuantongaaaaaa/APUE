//
// Created by cliondev on 2026/8/18.
//

#ifndef APUE_MEMORY_H
#define APUE_MEMORY_H

#endif //APUE_MEMORY_H


typedef struct yblock {
    struct yblock* next;//next block
} yblock;

static int* pool_start = NULL;//memory start address
static size_t pool_size = 0;//pool size
static size_t block_size = 0;//block size
static yblock* free_list = NULL;//free block list


void yinit(int size,int block_size);

void* ymalloc() ;

void yfree(int *arr);


void ydestory();
