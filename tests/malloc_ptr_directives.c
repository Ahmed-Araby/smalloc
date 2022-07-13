#include <stdio.h>
#include "malloc.h"
void test1_GET_directive();
void test2_SET_directive();
void test3_BHEADER_directive();
void test4_BSIZE_directive();
void test5_BALLOC_directive();
void test6_BFOOTER_directive();
int 
main(){
    printf("----------------------------------------------------------------\n");
    test1_GET_directive();
    printf("\n");
    test2_SET_directive();
    printf("\n");
    test3_BHEADER_directive();
    printf("\n");
    test4_BSIZE_directive();
    printf("\n");
    test5_BALLOC_directive();
    printf("\n");
    test6_BFOOTER_directive();
    printf("----------------------------------------------------------------\n");
}

void test1_GET_directive(){
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


void test2_SET_directive(){
    int arr[4] = {0, 0, 0, 0};
    int *ptr = arr + 3;
    SET((void *)ptr, 333);
    if(arr[3] == 333){
        printf("\033[0;32m"); // green
        printf("[test2_SET_directive succeded], arr[3] = %d\n", arr[3]);
    }
    else {
        printf("\033[0;31m"); // red
        printf("test2_SET_directive failed, arr[3] = %d \n", arr[3]);
    }
    printf("\033[0m"); // default
}

void test3_BHEADER_directive(){
    int arr[4] = {1, 2, 3, 4};
    void *ptr = (void *)(arr + 1);
    void *eh = arr;
    void * ah = BHEADER(ptr);
    if(eh == ah && *(unsigned int *)ah == 1){
        printf("\033[0;32m"); // green
        printf("[test3_BHEADER_directive succeded], *ah = %d\n", *(unsigned int *)ah);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test3_BHEADER_directive failed, ah = %p, *ah = %d \n", ah, *(unsigned int *)ah);
    }
}

void test4_BSIZE_directive(){
    /**
     * we need to assign correct block header to arr[0]
     */
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    int arr[4] = {header, 2, 3, header};
    void *ptr = (void *)(arr + 1); // payload pointer
    unsigned int asize = BSIZE(ptr);
    if(asize == 0x10){
        printf("\033[0;32m"); // green
        printf("[test4_BSIZE_directive succeded], asize = %d\n", asize);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test4_BSIZE_directive failed, asize = %d ", asize);
    }
}

void test5_BALLOC_directive(){
    /**
     * we need to assign a correct block header to arr[0]
     */
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    int arr[4] = {header, 2, 3, header}; // fotter is a replica of the footer
    void *ptr = (void *)(arr + 1); // payload pointer
    unsigned int aalloc = BALLOC(ptr);
    if(aalloc == 1){
        printf("\033[0;32m"); // green
        printf("[test5_BALLOC_directive succeded], aalloc = %d\n", aalloc);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test5_BALLOC_directive failed, aaloc = %d ", aalloc);
    }
}

void test6_BFOOTER_directive(){
    /**
     * we need to assign a correct block header to arr[0]
    */
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[4] = {header, 2, 3, footer}; // fotter is a replica of the footer
    
    void * ef = (void *) (arr + 3); 
    void *ptr = (void *)(arr + 1); // payload pointer
    void * af = BFOOTER(ptr);
    if(af == ef && *(unsigned int *)af == 0x11){
        printf("\033[0;32m"); // green
        printf("[test6_BFOOTER_directive succeded], *af = %d \n", *(unsigned int *)af);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test6_BFOOTER_directive failed, ef = %p, af= %p, *af = %d \n", ef, af, *(unsigned int *)af);
    } 
}