#ifndef MALLOC
#define MALLOC

/**
 * macros helpers to manipulate the pointers
 * of the heap blocks
 * 
 * ptr will always be a void pointer to 
 * the payload of a block within the heap
 */

// [TODO] this stuff should be configurable (use the argv)
// word size in bytes
#define WSIZE 4
#define DWSIZE 8
#define EXP_CHUNK (8 * DWSIZE)
//

// get / set word at pointer
#define GET(tptr) (* (unsigned int*) tptr)
#define SET(tptr, val) (*(unsigned int*) tptr) = val 
//

#define BHEADER(ptr) (void *) (ptr - WSIZE)
#define BSIZE(ptr) (unsigned int) (GET(BHEADER(ptr)) & ~0x7)
#define BALLOC(ptr) (unsigned int) (GET(BHEADER(ptr)) & 0x1)
#define BFOOTER(ptr) (void *) (ptr + BSIZE(ptr) - DWSIZE)
//

// navigation
#define BNEXT(ptr) (void *) (ptr + BSIZE(ptr))
#define BPREV(ptr) (void *) ( ptr - ( GET((ptr - DWSIZE)) & ~0x7 ) ) 
//

/**
 * header directives 
 * hptr points at the header of the heap block
 */
#define HSIZE(hptr) (unsigned int) (GET(hptr) & ~0x7)
#define HALLOC(hptr) (unsigned int) (GET(hptr) & 0x1)
#define HNEXT(hptr) (void *) (hptr + HSIZE(hptr))

///////////////////////////////////////////////////////////

// API signature to be used by the application code
void* mmalloc(unsigned int size);
void mfree(void *ptr);
//


// private helper methods used by the allocator to manage the heap
int hinit();
void merge(void*, void*);
void coalesce(void* ptr);
int  extendh();
void split(void *, unsigned int);
void* allocate(unsigned int);
int deallocate(void *);
//

#endif