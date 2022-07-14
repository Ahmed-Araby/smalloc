#include <stdio.h>
#include "../malloc.h"

void test1_GET_directive();
void test2_SET_directive();
void test3_BHEADER_directive();
void test4_BSIZE_directive();
void test5_BALLOC_directive();
void test6_BFOOTER_directive();
void test7_BPREV_directive();
void test8_BNEXT_directive();
// header directives
void test9_HSIZE_directive();
void test10_HALLOC_directive();
void test11_HNEXT_directive();

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
    printf("\n");
    test7_BPREV_directive();
    printf("\n");
    test8_BNEXT_directive();
    printf("\n");
    test9_HSIZE_directive();
    printf("\n");
    test10_HALLOC_directive();
    printf("\n");
    test11_HNEXT_directive();
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
    printf("\033[0m"); // default

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
    printf("\033[0m"); // default

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
    printf("\033[0m"); // default

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
    printf("\033[0m"); // default

}

void test7_BPREV_directive(){
    /**
     * we need to assign a correct block header to arr[0]
    */
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[12] = {
        0, 0x9, 0x9, 
        header, 2, 3, footer , 
        header, 8, 9, footer, 
        0x1
    }; // fotter is a replica of the footer
    void *_2nd_block_ptr = arr + 8; // points at the payload of the 2nd block
    void *eptr = arr + 4; // points at the payload of the 1st block
    void* aptr = BPREV(_2nd_block_ptr);
    if(arr[8] == 8 && aptr == eptr && *(unsigned int *)aptr == 2){
        printf("\033[0;32m"); // green
        printf("[test7_BPREV_directive succeded], *aptr = %d \n", *(unsigned int *)aptr);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test7_BPREV_directive failed, eptr = %p, aptr= %p, *aptr = %d \n", eptr, aptr, *(unsigned int *)aptr);
    } 
    printf("\033[0m"); // default

}

void test8_BNEXT_directive(){
    /**
     * we need to assign a correct block header to arr[0]
    */
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[12] = {
        0, 0x9, 0x9, 
        header, 2, 3, footer , 
        header, 8, 9, footer, 
        0x1
    }; // fotter is a replica of the footer
    void* _1st_block_ptr = arr + 4; // points at the payload of the 2nd block
    void* eptr = arr + 8; // points at the payload of the 1st block
    void* aptr = BNEXT(_1st_block_ptr);
    if(arr[4] == 2 && aptr == eptr && *(unsigned int *)aptr == 8){
        printf("\033[0;32m"); // green
        printf("[test8_BNEXT_directive succeded], *aptr = %d \n", *(unsigned int *)aptr);
    }
    else{
        printf("\033[0;31m"); // red
        printf("test8_BNEXT_directive failed, eptr = %p, aptr= %p, *aptr = %d \n", eptr, aptr, *(unsigned int *)aptr);
    } 
    printf("\033[0m"); // default

}

/**
 * header directives
 */
void test9_HSIZE_directive(){
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[12] = {
        0, 0x9, 0x9, 
        header, 2, 3, footer ,  // allocated block
        header & ~0x1, 8, 9, footer & ~0x1, // free block 
        0x1
    };
    void* b1hptr = arr + 3;
    void* b2hptr = arr + 7;
    const unsigned int ab1size = HSIZE(b1hptr);
    const unsigned int ab2size = HSIZE(b2hptr);
    if(ab1size == 0x10 && ab2size == 0x10){
        printf("\033[0;32m"); // green
        printf("[test9_HSIZE_directive succeded] \n");
    }
    else {
        printf("\033[0;31m"); // red
        printf("[test9_HSIZE_directive failed] ab1size = %d, ab2size = %d \n", ab1size, ab2size);
    }
    printf("\033[0m"); // default
}

void test10_HALLOC_directive(){
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[12] = {
        0, 0x9, 0x9, 
        header, 2, 3, footer ,  // allocated block
        header & ~0x1, 8, 9, footer & ~0x1, // free block 
        0x1
    };
    void* b1hptr = arr + 3;
    void* b2hptr = arr + 7;
    const unsigned int ab1alloc = HALLOC(b1hptr);
    const unsigned int ab2alloc = HALLOC(b2hptr);
    if(ab1alloc == 0x1 && ab2alloc == 0x0){
        printf("\033[0;32m"); // green
        printf("[test10_HALLOC_directive succeded] \n");
    }
    else {
        printf("\033[0;31m"); // red
        printf("[test10_HALLOC_directive failed] ab1size = %d, ab2size = %d \n", ab1alloc, ab2alloc);
    }
    printf("\033[0m"); // default
}

void test11_HNEXT_directive(){
    const unsigned int header = 0x11; // size = 16 bytes, and the block allocated so the LSb is 1
    const unsigned int footer = 0x11;
    int arr[12] = {
        0, 0x9, 0x9, 
        header, 2, 3, footer ,  // allocated block
        header & ~0x1, 8, 9, footer & ~0x1, // free block 
        0x1
    };
    void* hptr = arr + 3;
    void* nhptr = HNEXT(hptr);
    if(nhptr == arr + 7){
        printf("\033[0;32m"); // green
        printf("[test11_HNEXT_directive succeded] \n");
    }
    else {
        printf("\033[0;31m"); // red
        printf("[test11_HNEXT_directive failed] nhptr = %p \n", nhptr);
    }
    printf("\033[0m"); // default
}