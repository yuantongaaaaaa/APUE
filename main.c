//
// Created by cliondev on 2026/8/18.
//
#include <stdio.h>
#include "memory.h"


int main() {
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

    return 0;
}