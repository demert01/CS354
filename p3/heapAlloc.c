///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        heapAlloc.c
// This File:        heapAlloc.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Fall 2019
//
// Author:           Ye Ji Kim
// Email:            ykim762@wisc.edu
// CS Login:         ykim
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {
        int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */

blockHeader *heapStart = NULL;
blockHeader *prevHeap = NULL;
blockHeader *endHeap;

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {

    if(size <= 0)
	return NULL;

    // add blockHeader to the size
    int blockSize = size + sizeof(blockHeader);

    // check if the size is multiple of 8
    if(blockSize % 8 != 0)
	blockSize += (8 - blockSize % 8); // make size as multiple of 8

    // first time allocating
    if(prevHeap == NULL)
	prevHeap = heapStart;

    // starting from previous point - next fit
    blockHeader *cur = prevHeap;

    int valid = 0; // for while loop
    while(valid == 0){
	int ss = cur -> size_status >> 3 << 3; // get the size of header

	if((cur -> size_status & 1) != 0) { // if not free
	    cur = (blockHeader*)((void*)cur + ss); // next header
	    if(cur -> size_status == 1) { // reach to the end mark
		cur = heapStart; // start from heapStart again
	    }
	} else { // free

	    if(blockSize > ss) { // not enough space
	    	cur = (blockHeader*)((void*)cur + ss); // next header
	    } else { // enough space - break while loop	   
	        valid = 1;
		break;
	    }
    	}
	//check block have gone through whole block
	if(cur == prevHeap) {
	    return NULL;
	}
    }

    //calculate size to store	    
    int ss = cur -> size_status >> 3 << 3;
    int freesize = ss - blockSize;

    // check if rest of size is enought for spliting
    if(freesize >= 8) {
 
	// split
	blockHeader *next = (blockHeader*)((void*)cur + blockSize);

	next -> size_status = freesize; // size of next block
	next -> size_status += 2; // updat p bit

	// change size and status of cur block
	cur -> size_status = (cur -> size_status & 2) + blockSize;
	cur -> size_status += 1; // alloc

	// update footer of next block
	blockHeader *footer = (blockHeader*)((void*)next + freesize - 4);
	footer -> size_status = freesize;
	
    } else{ // exact size to store
	// change size	
	cur -> size_status += 1;
    }

    prevHeap = cur; // update previous pointer - next fit

    return ((void*)cur) + sizeof(blockHeader); // return after header
}

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {         
    
    if(ptr == NULL)
	return -1;

    // find the end of the heap
    blockHeader *end = heapStart;
    int valid = 0;
    while(valid == 0) {
	int ss = end -> size_status >> 3 << 3;
	if(end -> size_status != 1) {
	    end = (blockHeader*)((void*)end + ss);
	} else {
	    endHeap = end;
	    valid = 1;
	}
    }
    
    // get the header of block;
    blockHeader *point = (blockHeader*)((void*)ptr - sizeof(blockHeader));
    // get size of block from header
    int size = point -> size_status >> 3 << 3;

    if(size%8 != 0) // ptr not muliple of 8
	return -1;
 
    if(point < heapStart || point > endHeap) // outside of heap space
	return -1;
  
    if((point -> size_status & 1) == 0)   // already freed
        return -1;

    // mark block as free
    point -> size_status -= 1;
    // footer of cur block
    blockHeader *cfooter = (blockHeader*)((void*)point + size -4);
    cfooter -> size_status = size;

    int freeSize = size; // for footer
    int noprev = 0; // check prev block is coalese

    // check p bit
    int p = (point -> size_status) & 2;

    blockHeader *prev = NULL;
    if(p == 0) { // prev block is free
        // get to the footer of prev block
        blockHeader *pfooter = (blockHeader*)((void*)point - 4);

        int prevsize = pfooter -> size_status;
        // get prev block header
        prev = (blockHeader*)((void*)pfooter - prevsize + 4);

        freeSize += prevsize; // update freeSize
        prev -> size_status += freeSize-prevsize; // update prev header

        // update footer
        blockHeader *pnfooter = (blockHeader*)((void*)prev + freeSize - 4);
        pnfooter -> size_status = freeSize;

        prevHeap = prev; // update previous pointer - next fit
    } else {
	noprev = 1; // prev coalese not occur
    }
	
    // find the header of next block
    blockHeader *next;
    if(noprev) {
        next = (blockHeader*)((void*)point + size);
    } else { // prev block coalese
	next = (blockHeader*)((void*)prev + freeSize);
    }

    // change the p bit of next block
    next -> size_status -= 2;

    // check next block status
    int a = (next -> size_status) & 1;
    if(a == 0) { // next block empty	
	if(next -> size_status != 1) { //check if not endmark
	    // get the size of next block
	    int nextsize = next -> size_status >> 3 << 3;
	    freeSize += nextsize; // update freeSize

	    if(noprev) {
		// update the size of header
		point -> size_status += freeSize - size;

 		// update footer
		blockHeader *nfooter = (blockHeader*)((void*)next + nextsize - 4);
		nfooter -> size_status = freeSize;

		prevHeap = point; // update prev point - next fit
	    } else {
		// size of prev block
		int presize = prev -> size_status >> 3 << 3;
		// update the size of header
		prev -> size_status += freeSize - presize;

		// update footer
		blockHeader *nfooter = (blockHeader*)((void*)next + nextsize - 4);
		nfooter -> size_status = freeSize;

		prevHeap = prev; // update prev point - next fit
	    }
	}
    }

    return 0;    
}

/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {         
    
    static int allocated_once = 0; //prevent multiple initHeap calls

    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    int allocsize; // size of requested allocation including padding
    void* mmap_ptr; // pointer to memory mapped area
    int fd;
  
    blockHeader* endMark;

    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((char*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
}         
                 
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void DumpMem() {  

    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
}  
