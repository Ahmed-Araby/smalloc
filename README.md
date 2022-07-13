# malloc
writing my own explicit dynamic memory allocation library 

## dynamic virtual memory allocation / deallocation
this could be done in servral ways using one of the following sys calls
  * mmap
  * munmap 
  * sbrk
  
other more conveient / safe / reliable / portable approach is to use the malloc package provided by the std lib in the C language
this package use the sbrk sys call to extend / shrink the heak

## heap
the kernal keep an intenral private pointer 'brk' which point at the top of the heap, 
the heap is the area that comes after the .bss area in the virtual addres space it has an initial size = 0 bytes
