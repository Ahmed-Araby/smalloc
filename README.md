# malloc
Explicit Dynamic Memory Allocator, just like the malloc package in the C Langauge std library.
# Contents
- [What is dynamic memory allocation](#what-is-dynamic-memory-allocation)
- [Design](#design)
- [How to use this lib with your code](#usage)
- [To Do](#todo)

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

# Usage

# TODO
