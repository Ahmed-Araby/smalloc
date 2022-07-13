#include <stdio.h>
#include "malloc.h"
void test1_hinit();
int 
main(){
    printf("----------------------------------------------------------------\n");
    printf("\n");
    printf("----------------------------------------------------------------\n");
}

void test1_hinit(){
    int arr[4] = {55, 22, 10, 55}; // double word alligned block of heap
    unsigned int val = GET(arr);
    if(val == 55){
        printf("\033[0;32m"); // green
        printf("[test1_GET_directive succeded], required val = %d, actual val = %d \n", arr[0], val);
    }
    else {
        printf("\033[0;31m"); // red
        printf("test1_GET_directive failed, required val = %d, actual val = %d \n", arr[0], val);
    }
    printf("\033[0m"); // default
}
