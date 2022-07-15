#include <unistd.h>
#include <stdio.h>
#include "malloc.h"

// global variables
void *hb = NULL;
void *mbrk = NULL;
// 

/**
 * public API
 */

void* mmalloc(unsigned int size){
    return allocate(size);
}

/**
 * private helper methods
 */



/**
 * @brief will be called only the very first time, 
 * on which the application request to allocate dynamic memory
 */

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
    return 0;
}


/**
 * [TODO] write unit tests
 */
void merge(void* leftb, void* rightb){
    SET(BHEADER(leftb), (BSIZE(leftb) + BSIZE(rightb)));
    SET(BFOOTER(rightb), BSIZE(leftb));
}

void coalesce(void* ptr){
    if(BALLOC(ptr) != 0){
        printf("error: heap block pointed to be ptr  = %p should be free", ptr);
        return;
    }

    void* nexthbptr = BNEXT(ptr);
    void* prevhbptr = BPREV(ptr);
    if(BALLOC(nexthbptr) == 0){
        merge(ptr, nexthbptr);
    }
    if(BALLOC(prevhbptr) == 0){
        merge(prevhbptr, ptr);
    }
}

/**
 * @brief extend the heap by EXP_CHUNK(=64byte) at a time
 * extend must coalesce the new are with the are located before 
 * the epilogue if possible.
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


/**
 * @brief allocate 'size' of bytes to the application from the heap memory, 
 * [placement algorithm is 'first fit']
 * [always split the extra memory of the newly allocated block and make new free block if possible]
 
 * @param size size to be allocated from the heap memory, allocator must ensure that it is 
            multiple of the DWSIZE directive, to make sure that all blocks (allocated/free) are DWSIZE alligned
 * @return void* pointer to the first block of the allocated block or NULL if failed to allocate memory 
 */
void* allocate(unsigned int size){
    if(mbrk == NULL)
        hinit();

    // [TODO] this could be done with bit ops
    if(size % DWSIZE != 0){
        size = (size / DWSIZE + 1) * DWSIZE;
    }
    
    /**
     * points at EPILOGUE block if this is the very first allocate request, 
     * other wise it will point at the header of the first regular block. 
     */
    void *hptr = hb + 3 * WSIZE;
    while(1){
        if(GET(hptr) == 0x1)
        { // EPILOGUE BLOCK HEADER
            
            int ret = extendh();
            if(ret == -1){
                printf("error: failed to allocate %d bytes\n", size);
                return NULL;
            }
            return allocate(size); // ISA, this MUST succed
        }
        else if(
            HALLOC(hptr) == 0 &&
            HSIZE(hptr) >= size + 2 * WSIZE
        )
        { // fit free area
            
            /**
             * points at the first block in the payload
             * of this fit free area 
             */
            void* ptr = hptr + WSIZE;  
            split(ptr, size);
            unsigned int header = GET(BHEADER(ptr));
            SET(BHEADER(ptr), header | 0x1);
            SET(BFOOTER(ptr), header | 0x1);
            return ptr;
        }
        else{
            hptr = HNEXT(hptr);
        } 
    }
}

/**
 * @brief we could keep track of the pointers handed out
 * to the application to make sure that deallocation do
 * not miss the heap memory foot prints when the application, 
 * try to free an area by passing an invalid pointer, 
 * what is an invaid pointer ? 
 * any pointer that do not point at the first 
 * block of a regualr allocated area is invalid.
 * but this book keeping will make freeing heap block more
 * expensive, so for now it is the responsibility of
 * the person who writes the application code to, make sure
 * that the pointer passed to free(the public API method that will call deallocate) 
 * is a valid pointer.
 * 
 * @param ptr supposed to be pointer to the first block of 
 * a regular allocated heap area.
 * what is regular ? 
 * regular heap area is any area but 
 *  1- very first WORD of the heap (not ever used)
 *  2- the prologue block
 *  3- the epilogue block
 */

int deallocate(void* ptr){
    if(!(ptr >= hb && ptr < mbrk)){
        printf("error: pointer do not lie within the heap \n");
        return -1; 
    }
    if(
        (BHEADER(ptr)>= hb && BHEADER(ptr) < mbrk) &&
        (BFOOTER(ptr) >= hb && BFOOTER(ptr) < mbrk)
    ){
        const unsigned int header = GET(BHEADER(ptr));
        SET(BHEADER(ptr), (header & ~0x1));
        SET(BFOOTER(ptr), (header & ~0x1));
        coalesce(ptr);
        return 0;
    }
    else {
        printf("error: ptr do not point at a valid heap block \n");
        return -1;
    }
}