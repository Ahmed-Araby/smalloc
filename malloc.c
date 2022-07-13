#include <unistd.h>
#include <stdio.h>
#include "malloc.h"
/**
 * @brief will be called only the very first time, 
 * on which the application request to allocate dynamic memory
 */

// global variables
void *hb = NULL;
void *mbrk = NULL;
// 

int hinit(){
    hb = sbrk(0);
    int ret = brk(hb + 4 * WSIZE);
    if(ret != 0){
        printf("error: failed to initialize heap of 4 bytes");
        return -1;
    }
    mbrk = hb + 4 * WSIZE;

    // [--][8/1][8/1][0/1]  (current heap)
    SET(hb, 0);
    SET((hb + WSIZE), 0x9);
    SET((hb + DWSIZE), 0x9);
    SET((hb + 3 * WSIZE), 0x1);
    //

    printf("hb = %p, mbrk = %p \n ", hb, mbrk);
    return 0;
}


// public API
void *mmalloc(unsigned int size){
    if(hb == NULL)
        hinit();
    return hb;
}

