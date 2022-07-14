#include <unistd.h>
#include <stdio.h>
#include "malloc.h"

// global variables
void *hb = NULL;
void *mbrk = NULL;
// 

/**
 * private helper methods
 */

void coalesce(void* ptr){

}

/**
 * @brief extend the heap by EXP_CHUNK(=64byte) at a time
 * 
 * @return int 0 is succeeded, -1 if failed
 */
int extendh(){
    if(mbrk == NULL){
        mbrk = sbrk(0);
    }
    int ret = brk(mbrk + EXP_CHUNK);
    if(ret == 0){
        SET(BHEADER(mbrk), EXP_CHUNK);
        SET(BFOOTER(mbrk), EXP_CHUNK);
        SET((mbrk + EXP_CHUNK - WSIZE), 1); // EPILOGUE block
        coalesce(mbrk);
        mbrk = mbrk + EXP_CHUNK;
        return 0;
    }
    else
        return -1;
}

/**
 * @brief split the heap block pointed to by ptr into two blocks,
 * left block which has payload of size 'size' in bytes and, 
 * right block with has the remaining size (if exist) of the orignal block, 
 * because the original block is dword alligned and 'size' is multiple of dword, 
 * it is garanted that left and right block (if exist) both will be dword alligned.
 * 
 * @param ptr pointer to the payload of the heap block to be splited
 * @param size size of the payload to be allocated in bytes (must be multiple of double word (=8))
 */
void split(void* ptr, unsigned int size) {
    const unsigned int bsize = BSIZE(ptr);
    const unsigned int rbsize  = bsize - size - DWSIZE;
    
    // RIGHT BLOCK
    const void* lh = BHEADER(ptr);
    SET(lh, (size + DWSIZE));
    const void* lf = BFOOTER(ptr);
    SET(lf, (size + DWSIZE));
    //
    // LEFT BLOCK
    const void* rh = lh + size + DWSIZE;
    SET(rh, rbsize);
    const void *rf = BFOOTER((rh + WSIZE));
    SET(rf, rbsize);
    //
}
