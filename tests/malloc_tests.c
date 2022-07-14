#include <stdio.h>
#include <unistd.h>
#include "../malloc.h"

void test1_split_left_block();
void test1_split_right_block();
void test3_extendh();
void test4_extendh_check_new_pos_of_epilogue();
void test5_coalesce_withprev();
void test6_coalesce_withnext();
void test7_coalesce_no_coalesce();
void test8_coalesce_prev_and_next(); 
void test9_hinit();

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
    printf("\n");
    test5_coalesce_withprev();
    printf("\n");
    test6_coalesce_withnext();
    printf("\n");
    test7_coalesce_no_coalesce();
    printf("\n");
    test8_coalesce_prev_and_next(); 
    printf("\n");
    test9_hinit();

    // test4_extendh_check_new_pos_of_epilogue();
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
    /**
     * @brief this test do not simulate real senario, 
     * as the very initial heap should be initiated manually with 
     * PROLOGUE block and EPILOGUE BLOCK
     */
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

void test5_coalesce_withprev(){
    const unsigned int header = 0x10; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x10;
    int arr[12] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, footer, 
        header, 5, 6, footer, 
        0x1 // EPILOGUE BLOCK
    };
    void* ptr = arr + 8; 
    coalesce(ptr);
    void* nptr = arr + 4;
    if(
        GET(BHEADER(nptr)) == 0x20 && 
        BFOOTER(nptr) == arr + 10 && 
        GET(BFOOTER(nptr)) == 0x20 && 
        arr[11] == 0x1 
    ){
        printf("\033[0;32m"); // green
        printf("[test5_coalesce_withprev] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test5_coalesce_withprev] failed \n");
    }
    printf("\033[0m"); // default
}

void test6_coalesce_withnext(){
    const unsigned int header = 0x10; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x10;
    int arr[12] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, footer, 
        header, 5, 6, footer, 
        0x1 // EPILOGUE BLOCK
    };
    void* ptr = arr + 4; 
    coalesce(ptr);
    void* nptr = arr + 4;
    if(
        GET(BHEADER(nptr)) == 0x20 && 
        BFOOTER(nptr) == arr + 10 && 
        GET(BFOOTER(nptr)) == 0x20 && 
        arr[11] == 0x1 
    ){
        printf("\033[0;32m"); // green
        printf("[test6_coalesce_withnext] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test6_coalesce_withnext] failed \n");
    }
    printf("\033[0m"); // default
}


void test7_coalesce_no_coalesce(){
    const unsigned int header = 0x10; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x10;
    int arr[12] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, footer, 
        header | 0x1, 5, 6, footer | 0x1,  // allocated area 
        0x1 // EPILOGUE BLOCK
    };
    void* ptr = arr + 4; 
    coalesce(ptr);
    void* nptr = arr + 4;
    if(
        GET(BHEADER(nptr)) == 0x10 && 
        BFOOTER(nptr) == arr + 6 && 
        GET(BFOOTER(nptr)) == 0x10 && 
        arr[11] == 0x1 
    ){
        printf("\033[0;32m"); // green
        printf("[test7_coalesce_no_coalesce] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test7_coalesce_no_coalesce] failed \n");
    }
    printf("\033[0m"); // default
}

void test8_coalesce_prev_and_next(){
    const unsigned int header = 0x10; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x10;
    int arr[16] = {
        0,
        0x9, 0x9,  // PROLOGUE BLOCK
        header, 1, 2, footer, 
        header, 5, 6, footer, 
        header, 7, 8, footer, 
        0x1 // EPILOGUE BLOCK
    };
    void* ptr = arr + 8; 
    coalesce(ptr);
    void* nptr = arr + 4;
    if(
        GET(BHEADER(nptr)) == 0x30 && 
        BFOOTER(nptr) == arr + 14 && 
        GET(BFOOTER(nptr)) == 0x30 && 
        arr[15] == 0x1 
    ){
        printf("\033[0;32m"); // green
        printf("[test8_coalesce_prev_and_next] succeded \n");
    }
    else{
        printf("\033[0;31m"); // red
        printf("[test8_coalesce_prev_and_next] failed \n");
    }
    printf("\033[0m"); // default
}

void test9_hinit(){
    const void* ehb = sbrk(0);
    const void* embrk = ehb + 4 * WSIZE;
    hinit();
    unsigned int arr[4] = {0, 0x9, 0x9, 0x1};
    void* eheap = arr;
    if(ehb != hb || embrk != mbrk){
        printf("\033[0;31m"); // red
        printf("[test9_hinit] failed ehb = %p, hb = %p, embrk = %p, mbrk = %p \n", ehb, hb, embrk, mbrk);
        printf("\033[0m"); // default
        return ;
    }
    for(int i=0; i<4; i++){
        if(GET((eheap + i * WSIZE)) != GET((hb + i * WSIZE))){
            printf("\033[0;31m"); // red
            printf("[test9_hinit] failed i = %d, eheap[i] = %d, hb[i] = %d \n", i, GET((eheap + i * WSIZE)), GET((hb + i * WSIZE)));
            printf("\033[0m"); // default
            return ;
        }
    }

    printf("\033[0;32m"); // green
    printf("[test9_hinit] succeded \n");
    printf("\033[0m"); // default
}