#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"
void test1_split_left_block();
void test1_split_right_block();
void test3_extendh();
int 
main(){
    printf("----------------------------------------------------------------\n");
    test1_split_left_block();
    printf("\n");
    test1_split_right_block();
    printf("\n");
    test3_extendh();
    printf("----------------------------------------------------------------\n");
}

void test1_split_left_block(){
    const unsigned int header = 0x20; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x20;
    const unsigned int size = 8;
    int arr[12] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, 3, 4, 5, 6, footer, 
        0x1 // EPILOGUE BLOCK
    };

    void* bptr = arr + 4; // pointer to the payload of the right block
    split(bptr, size);
    
    const unsigned int lh = 0x10;
    const unsigned int lf = 0x10;
    const void *bheader = BHEADER(bptr);
    const void * bfooter = BFOOTER(bptr);
    if(
        // left block checks
        bheader == arr + 3 &&
        bfooter == arr + 6 && 
        GET(bheader) == lh &&
        GET(bfooter) == lf && 
        *(unsigned int*)bptr == 1
    ){
        printf("\033[0;32m"); // green
        printf("[test1_split_left_block] succeded \n");
    }
    else {
        printf("\033[0;31m"); // red
        printf("[test1_split_left_block failed], arr = %p, bheader = %p, *bheader = %d, bfooter = %p, *bfooter = %d, *bptr = %d \n", arr, bheader,  *(unsigned int *)bheader, bfooter, *(unsigned int *) bfooter, *(unsigned int *)bptr);
    }
    printf("\033[0m"); // default
}


void test1_split_right_block(){
    const unsigned int header = 0x20; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x20;
    const unsigned int size = 8;
    int arr[12] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, 3, 
        4, 5, 6, footer, 
        0x1 // EPILOGUE BLOCK
    };

    void* bptr = arr + 4; // pointer to the payload of the right block
    split(bptr, size);
    
    void * lbptr = arr + 8;
    const unsigned int rh = 0x10;
    const unsigned int rf = 0x10;
    const void *bheader = BHEADER(lbptr);
    const void * bfooter = BFOOTER(lbptr);
    if(
        // left block checks
        bheader == arr + 7 &&
        bfooter == arr + 10 && 
        GET(bheader) == rh &&
        GET(bfooter) == rf && 
        *(unsigned int*)lbptr == 5
    ){
        printf("\033[0;32m"); // green
        printf("[test1_split_right_block] succeded \n");
    }
    else {
        printf("\033[0;31m"); // red
        printf("[test1_split_right_block failed], arr = %p, bheader = %p, *bheader = %d, bfooter = %p, *bfooter = %d, *bptr = %d \n", arr, bheader,  *(unsigned int *)bheader, bfooter, *(unsigned int *) bfooter, *(unsigned int *)bptr);
    }
    printf("\033[0m"); // default
}


void test3_extendh(){
    void* brk = sbrk(0);
    void* ebrk = brk + EXP_CHUNK;
    int ret = extendh();
    void* abrk = sbrk(0);
    if(ret == 0 && ebrk == abrk && abrk - brk == EXP_CHUNK){
        printf("\033[0;32m"); // green
        printf("[test3_extendh] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test3_extendh] failed, brk = %p, ebrk = %p, abrk = %p, EXP_CHUNK = %d \n", brk, ebrk, abrk, EXP_CHUNK);
    }
}