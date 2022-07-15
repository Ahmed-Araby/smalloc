# malloc
Explicit Dynamic Memory Allocator, just like the malloc package in the C Langauge std library.
# Contents
- [What is dynamic memory allocation](#what-is-dynamic-memory-allocation)
- [Design](#design)
- [How to use this lib with your code](#usage)
- [my take on some of the weird details in the implementation](#implementation-details)
- [To Do](#todo)
- [references and material ](#references-and-material)

# What is dynamic memory allocation
dynamic memory refer to the so called 'heap' and it is the area that comes after the .bss (un initialized data area) and grow,
in the direction of increasing memory addresses.

![memory layout](https://github.com/Ahmed-Araby/malloc/blob/main/images/memory-layout.png)

the kernal keeps an internal pointer called 'brk' per process to track the heap top, at the very begining (when the process just started before any dynamic memory allocation requests) the size of the heap is zero bytes and 'brk' points at the '.bss' area top.

linux provide a sys call called 'brk' which could be used to extend / shrink the heap, how ever such use is not conventional, as there is more to do with the heap than just extending or shrinking.
as we know (and do, in our day to day use of relatively low level langauge like c and c++) we can allocate different size area of the heap to our usage,
and free theses areas when ever we are done with them, so we need to re-use the area we freed for future allocations, unless there is no free area within 
the heap we can ask the kernal to for more memory to extend the heap.

so there is alot to do with the heap memory
 * allocate memory from the heap for our application code use
 * free the memory used by the application code when application code wants.
 * re-use the previously freed memory when possible for future memory allocation request
 * ask the kernal for more memory to extend the heap when needed.

and all of this has to be done in efficient(fast) manner and make maximum possible utilization of the heap memory(reduce external/internal fragmentation).

so we need some kind of reusbale code to take care of theses functionalities for our application code, and this is called "Dynamic Memory Allocators", 
for example the C programming lanague provide package(as part of the std library) called 'malloc' that manage the heap memory.

internally this package will make use of wrapper functions brk/sbrk implemented by C std lib which by their turn will interact with the kernal
sys call 'brk', also it could make use of 'mmap' and 'munmap' sys calls, they can achieve the same goals

Dynamic Memory Allocators has 2 types 'Explicit' and 'Implicit':
* 'Explicit Dyanmic Memory Allocators' will free allocated areas from the heap to be re-usbale only when the application code asks for (this is called manual memory management).
* 'Implicit Dynamic Memory Allocators' will keep track of the references to the allocated memory and free the area that has no references any more (this is called garabe collection).

this project implements an "Explicit Dynamic Memory Allocator".

# Design
example of heap memory area with the footprints 
![heap structure](https://github.com/Ahmed-Araby/malloc/blob/main/images/heap-structure.png)

aspects of the design for an 'Explicit Dynamic Memory Allocator'
* how to keep track / manage the heap memory.
    
    in my implementation I used "implicit free list" which just a bunch of memory footprints that surround heap blocks (allocated / free), 
    and theses memory footprints contains meta data about the block (size and allocated or not) and we can use the size (works as implicit pointer)     to navigate the heap memory
    
* memory padding / allignment
  
    this allocator force the heap memory to be double word alligned where word equls 4 bytes and this mean that the size of any block (allocated / free) must be multiple of double word (=16 bytes) and block size include every thing about the block (header, footer, payload and padding), also allocation requests should be rounded up to the nerest multiple of double word, one of the reasons for such a constraint is to make sure that there is always a space for the header and footer as our header/footer are of word size each hence total is double word.
    
* what is the structure of the heap block (allocated or free).

![heap block structure](https://github.com/Ahmed-Araby/malloc/blob/main/images/heap-block-structure.png)

* algorithm used to find suitable block for allocation request.
    
    first fit alogirthm.
    
* how to handle internal fragmentation when the heap block to be allocated is bigger than the requestd size.
  
    split the extra memory into a new free block, and because the size to be allocated is multiple of double word and the current block area to be allocated has size that is multiple of double word, then the free size will also be multiple of double word, however we should only split when the extra area are more than double word other wise we will have free block with enough space for header and footer but no payload, also we should set some limit on the extra memory that we should split other wise we will end up with small free blocks which will make the heap navigation more expensive, as the complexity of navigating the heap is O(number of blocks).
    
* how to handle free requests to avoid falsy external fragementation.
    
    we should try to coalesce the block to be freed with the next neighbour block (if free) and the previous neighbour block (if free), coalescing is the act of merging 2 free blocks into one bigger free block.

* public API and internal functions that manage the heap 

    I made this diagram to help me with order on which I should start to implement the allocator functions, I guess it could give a clear idea about what to expect from the allocator public API and how does the internal works
![public API and internal functions diagram](https://github.com/Ahmed-Araby/malloc/blob/main/images/public-API-and-private-functions.jpg)

# Usage
with the follwing assumpations
* you run linux ( I use ubuntu)
* you have gcc installed 
* you have valgrind installed, why valgrind? we will use valgrind to make sure that memory locations we writing/reading to/from are actually allocated hence our Dynamic memory allocator works fine, as some times your c process can start writing/reading  to/from memory areas without beaing allocated and you could have no clue, also valgrind will tell us weather we have memory leaks or not hence we cam make sure that deallocate memory correctly.
* you cloned this repo
* your terminal sets in inside the main repo directory
* the file you wrote with the C code that will use this library is called "main.c" and located in the repo main directory (after you clone offcourse)

to allocate and free memory

```
#include "malloc.h"

int 
main(){
    int *arr = mmalloc(4 * sizeof(int)); // take size in bytes
    /*
    * you can play with the memory you have here
    * write/read to/from it and see valgrind output.
    * also you can comment the mfree(arr) line and see the memory leak report from valgrind.
    */
    mfree(arr);
}
```

then write the following in the termianl

```
gcc main.c malloc.c -Wall -o main
valgrind ./main
```

also there are some files that make use of the library in the "usage" folder

# Implementation Details

# TODO

# references and material 
