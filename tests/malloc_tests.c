#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"

void test1_split_left_block();
void test1_split_right_block();
void test3_extendh();
void test4_extendh_check_new_pos_of_epilogue();

extern void* hb;
extern void* mbrk;

int 
main(){
    printf("----------------------------------------------------------------\n");
    test1_split_left_block();
    printf("\n");
    test1_split_right_block();
    printf("\n");
    test3_extendh();
    printf("\n");
    test4_extendh_check_new_pos_of_epilogue();
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
    printf("\033[0m"); // default
}

void test4_extendh_check_new_pos_of_epilogue() {
    hb = sbrk(0);
    brk(hb + 4 * WSIZE);
    // [--][8/1][8/1][0/1]  (current heap)
    SET(hb, 0);
    SET((hb + WSIZE), 0x9);
    SET((hb + DWSIZE), 0x9);
    SET((hb + 3 * WSIZE), 0x1);
    mbrk = hb + 4 * WSIZE;
    int ret = extendh();
    void *ptr = hb + 4 * WSIZE;

    if(
        ret == 0 && 
        GET(BHEADER(ptr)) == EXP_CHUNK &&
        GET(BFOOTER(ptr)) == EXP_CHUNK && 
        GET((mbrk - WSIZE)) == 0x1
    ){
        printf("\033[0;32m"); // green
        printf("[test4_extendh_check_new_pos_of_epilogue] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test4_extendh_check_new_pos_of_epilogue] failed, ret = %d, *header = %d, *footer = %d, endblock = %d, footer = %p, end block = %p \n", ret, GET(BHEADER(ptr)), GET(BFOOTER(ptr)), GET((mbrk - WSIZE)), BFOOTER(ptr), (mbrk-WSIZE));
    }
    printf("\033[0m"); // default
}