#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

void *memorypool;

void *myallocate( int n){
    return sbrk(n);
}

void resetmemorypool(void){
    brk(memorypool);
}

void initmemorypool(void){
    memorypool = sbrk(0);
}

int main (void){
    initmemorypool();
    
    int *arr = (int*) myallocate (10* sizeof(int));
    if (arr == NULL){
        printf("myallocate error");
        return(1);
    }

    for (int i = 0; i<10 ; i++){
        arr[i] = i + 1;
    }

    printf("values of arr:\n");
    for(int i = 0; i<10;i++){
        printf("%d\n", arr[i]);
    }

    resetmemorypool();
    return (0);
}