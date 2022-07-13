#ifndef MALLOC
#define MALLOC

/**
 * macros helpers to manipulate the pointers
 * of the heap blocks
 * 
 * ptr will always be a void pointer to 
 * the payload of a block within the heap
 */
// word size in bytes
#define WSIZE 4
#define DWSIZE 8

// get / set word at pointer
#define GET(tptr) (* (unsigned int*) tptr)
#define SET(tptr, val) (*(unsigned int*) tptr) = val 

#define BHEADER(ptr) (void *) (ptr - WSIZE)
// size and allocation bit
#define BSIZE(ptr) (unsigned int) (GET(BHEADER(ptr)) & ~0x7)
#define BALLOC(ptr) (unsigned int) GET(BHEADER(ptr)) & 0x1
//
#define BFOOTER(ptr) (void *) (ptr + BSIZE(ptr) - DWSIZE)
/**
 * API signature to be used by the application code
 * */
void* mmalloc(unsigned int size);
void* mfree(void *ptr);
//


/**
 * @brief private helper methods
 * used by the allocator to manage the heap
 */
int hinit();
int  hsbrk();
void allocate();
void mb(void *ptr, int header); // make new block
void coalesce();
#endif