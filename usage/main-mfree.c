#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"

int 
main(){
    int *arr = mmalloc(4 * sizeof(int));
    // int *arr = sbrk(0); // this will write to unallocated memory 
    for(int i=0; i<4; i++){
        arr[i] = i * i;
    }
    for(int i=0; i<4; i++){
        printf("arr[%d] = %d \n", i, arr[i]);
    }
    
    mfree(arr);
    
    int *arr2 = mmalloc(4 * sizeof(int));
    if(arr == arr2){
        printf("first fit placement algorithm works \n");
    }
    return 0;
}