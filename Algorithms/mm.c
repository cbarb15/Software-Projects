#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

typedef struct
{
    size_t size;
    char allocated;
}block_header;

typedef struct
{
    size_t size;
    int filler_for_alignment;
}block_footer;

typedef struct
{
    void *next_chunk;
    void *prev_chunk;
}mem_list;

/* always use 16-byte alignment */
#define ALIGNMENT 16
#define INITIAL_SIZE  65536
//#define INITIAL_SIZE  8192
//#define INITIAL_SIZE  4096
#define YES 1
#define NO 0

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize()
 * PAGES are the chunks of memory in the slides and videos
 * In the slides a page was 8 cells in malloc7 video*/
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

/*Calculates block overhead*/
#define OVERHEAD (sizeof(block_header) + sizeof(block_footer))

/*Gets header pointer from a payload pointer*/
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))
/*Gets the footer pointer of the payload*/
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp))-OVERHEAD)

/*Gets size of block/chunk of memory and if it is allocated or not*/
#define GET_SIZE(p)  ((block_header *)(p))->size
#define GET_ALLOC(p) ((block_header *)(p))->allocated

/*Gets the next blocks payload pointer*/
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
/*Gets the previous blocks payload pointer*/
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp)-OVERHEAD))

/* rounds down to the nearest multiple of mem_pagesize() */
#define ADDRESS_PAGE_START(p) ((void *)(((size_t)p) & ~(mem_pagesize()-1)))

#define FIRST_BP_IN_MEMORY(bp) (bp + sizeof(mem_list) + OVERHEAD + sizeof(block_header))

int ptr_is_mapped(void *p, size_t len)
{
    void *s = ADDRESS_PAGE_START(p);
    return mem_is_mapped(s, PAGE_ALIGN((p + len) - s));
}

static void *allocate(int);
static void setUpMemChunk(void*);

void *startOfMemory = NULL;
void *startOfChunk = NULL;
int current_avail_size = 0;
void *block_ptr;
void *block_ptr_ftr;
void *prologue_ptr;
void *prologue_ftr;
void *sentinal_ptr;
void *start_mem_list;
int CHUNK_COUNT = 0;
int COALESCED = 0;
int smallestPayload = 65536;
/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  current_avail_size = PAGE_ALIGN(INITIAL_SIZE);
  //Current avail is the ptr to the start of my chunk of memory
  startOfChunk = mem_map(current_avail_size);

  startOfMemory = startOfChunk;

  start_mem_list = startOfChunk;
  ((mem_list *)(start_mem_list))->next_chunk = NULL;
  ((mem_list *)(start_mem_list))->prev_chunk = NULL;
  current_avail_size -= sizeof(mem_list);
  startOfChunk += sizeof(mem_list);

  //we get blank chunk of memory with mem
  //When request from mmap it
  //has to be increments of 4096
  //Remember to cast void*
  prologue_ptr = startOfChunk;
  prologue_ftr = startOfChunk + sizeof(block_header);
  ((block_footer *)(prologue_ftr))->size = OVERHEAD;
  GET_SIZE(prologue_ptr) = OVERHEAD;
  GET_ALLOC(prologue_ptr) = YES;
  current_avail_size -= OVERHEAD;

  sentinal_ptr = startOfChunk;
  sentinal_ptr += (OVERHEAD + current_avail_size - sizeof(block_header));
  GET_SIZE(HDRP(sentinal_ptr)) = 0;
  GET_ALLOC(HDRP(sentinal_ptr)) = YES;
  current_avail_size -= sizeof(block_header);

  //Set size of chunk of memory header and allocated and footer
  block_ptr = startOfChunk;
  block_ptr += (sizeof(block_header) * 2 + sizeof(block_header));
  GET_SIZE(HDRP(block_ptr)) = current_avail_size;
  GET_ALLOC(HDRP(block_ptr)) = NO;
  current_avail_size -= sizeof(block_header);

  //Set initial block footer size and allocation
  block_ptr_ftr = startOfChunk;
  block_ptr_ftr += (OVERHEAD + sizeof(block_header) + current_avail_size - sizeof(block_footer));
  ((block_footer *)(block_ptr_ftr))->size = current_avail_size + sizeof(block_header);
  current_avail_size -= sizeof(block_footer);

  CHUNK_COUNT = 1;

  return 0;
}

/*
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
//    int newSize = ALIGN(size + OVERHEAD);
  int newSize = ALIGN(size);
  void *ptr = NULL;

  //There is enough room in the current chunk of memory
  if(current_avail_size > newSize)
  {
    ptr = allocate(newSize);
    return ptr;
  }

  /*The chunk of memory is not big enough
    A new page needs to be allocated and linked with the existing chunk*/
  if(CHUNK_COUNT == 1)//if we are at the start of the linked list of memory chunks
  {
    void *tempPtr = start_mem_list;

    current_avail_size = PAGE_ALIGN(INITIAL_SIZE);
    startOfChunk = mem_map(current_avail_size);
    ((mem_list *)(start_mem_list))->next_chunk = startOfChunk;
    setUpMemChunk(tempPtr);
    ptr = allocate(newSize);
  }
  else
  {
    mem_list *tempPtr = startOfChunk - sizeof(mem_list);
    current_avail_size = PAGE_ALIGN(INITIAL_SIZE);
    startOfChunk = mem_map(current_avail_size);
    tempPtr->next_chunk = startOfChunk;
    setUpMemChunk(tempPtr);
    ptr = allocate(newSize);
  }

  return ptr;
}

void *allocate(int newSize)
{
  void *bp = startOfChunk + OVERHEAD + sizeof(block_header);
  void *best_bp = NULL;

  while(GET_SIZE(HDRP(bp)) != 0)
  {
    if(GET_ALLOC(HDRP(bp)) == NO && ((newSize + OVERHEAD) <= GET_SIZE(HDRP(bp))))
    {
      if(!best_bp || GET_SIZE(HDRP(bp)) < GET_SIZE(HDRP(best_bp)))
      {
        best_bp = bp;
      }
    }

    bp = NEXT_BLKP(bp);
  }

  if(best_bp != NULL)
    bp = best_bp;

  int oldBpSize = GET_SIZE(HDRP(bp));
  GET_SIZE(HDRP(bp)) = newSize + OVERHEAD;
  GET_ALLOC(HDRP(bp)) = YES;

  void *tempFtrPtr = bp;
  tempFtrPtr += newSize;
  ((block_footer *)(tempFtrPtr))->size = newSize + OVERHEAD;

  void *tempNewBlkHdr = tempFtrPtr + sizeof(block_header);
  current_avail_size -= (newSize + OVERHEAD);
  GET_SIZE(tempNewBlkHdr) = oldBpSize - (newSize + OVERHEAD);
  GET_ALLOC(tempNewBlkHdr) = NO;

  void *tempFtr2 = tempNewBlkHdr;
  tempFtr2 = FTRP(tempNewBlkHdr + sizeof(block_header));
  ((block_footer *)(tempFtr2))->size = current_avail_size + OVERHEAD;

  return bp;
}

void setUpMemChunk(void* oldChunk)
{
  void *prologue_ptr;
  void *sentinal_ptr;
  mem_list *memory_list;

  memory_list = startOfChunk;
  ((mem_list *)(memory_list))->next_chunk = NULL;
  ((mem_list *)(memory_list))->prev_chunk = oldChunk;
  current_avail_size -= sizeof(mem_list);
  startOfChunk += sizeof(mem_list);

  prologue_ptr = startOfChunk;
  prologue_ftr = startOfChunk + sizeof(block_header);
  ((block_footer *)(prologue_ftr))->size = OVERHEAD;
  GET_SIZE(prologue_ptr) = OVERHEAD;
  GET_ALLOC(prologue_ptr) = YES;
  current_avail_size -= OVERHEAD;

  sentinal_ptr = startOfChunk;
  sentinal_ptr += (OVERHEAD + current_avail_size - sizeof(block_header));
  GET_SIZE(HDRP(sentinal_ptr)) = 0;
  GET_ALLOC(HDRP(sentinal_ptr)) = YES;
  current_avail_size -= sizeof(block_header);

  //Set size of chunk of memory header and allocated and footer
  block_ptr = startOfChunk;
  block_ptr += (sizeof(block_header) * 2 + sizeof(block_header));
  GET_SIZE(HDRP(block_ptr)) = current_avail_size;
  GET_ALLOC(HDRP(block_ptr)) = NO;
  current_avail_size -= sizeof(block_header);

  //Set initial block footer size and allocation
  block_ptr_ftr = startOfChunk;
  block_ptr_ftr += (OVERHEAD + sizeof(block_header) + current_avail_size - sizeof(block_footer));
  ((block_footer *)(block_ptr_ftr))->size = current_avail_size + sizeof(block_header);
  current_avail_size -= sizeof(block_footer);

  CHUNK_COUNT++;
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  void *tempFtrPtr = NULL;
  void *bp_holder;
  void *bp_temp;
  int freeBpSize = GET_SIZE(HDRP(ptr)) - OVERHEAD;
  int total_block_size = GET_SIZE(HDRP(ptr));
  GET_ALLOC(HDRP(ptr)) = NO;

  bp_temp = ptr;
  /*1. First walk to the left of the block ptr until you find an un-allocated
   block or you reach the beginning of the chunk of memory
   2. If you find an un-allocated chunk(s) before you get to the beginning of the
   list, record their header sizes*/
  while(GET_SIZE(HDRP(PREV_BLKP(bp_temp))) != OVERHEAD)
  {
    bp_temp = PREV_BLKP(bp_temp);

    if(GET_ALLOC(HDRP(bp_temp)) == NO && GET_SIZE(HDRP(bp_temp)) > 0)
    {
      total_block_size += GET_SIZE(HDRP(bp_temp));
      COALESCED = YES;
    }
    else
    {
      bp_temp = NEXT_BLKP(bp_temp);
      break;
    }
  }
  bp_holder = bp_temp;
  bp_temp = ptr;
  /*1. Walk to the right of the ptr until you find an un-allocated block
    or you reach the end of the chunk of memory
    2. If you find an un-allocated chunk(s) before you get to the end of the
    list, record their header sizes*/
  while(GET_SIZE(HDRP(NEXT_BLKP(bp_temp))) != 0)
  {
    bp_temp = NEXT_BLKP(bp_temp);

    if(GET_ALLOC(HDRP(bp_temp)) == NO && GET_SIZE(HDRP(bp_temp)) > 0)
    {
      total_block_size += GET_SIZE(HDRP(bp_temp));
      COALESCED = YES;
    }
    else
    {
      bp_temp = PREV_BLKP(bp_temp);
      break;
    }
  }
  ptr = bp_holder;

  if(COALESCED)
    current_avail_size += (freeBpSize + OVERHEAD);
  else
    current_avail_size += freeBpSize;

  GET_SIZE(HDRP(ptr)) = total_block_size;
  GET_ALLOC(HDRP(ptr)) = NO;
  tempFtrPtr = FTRP(ptr);
  ((block_footer *)(tempFtrPtr))->size = total_block_size;

  COALESCED = NO;
}

/*
 * mm_check - Check whether the heap is ok, so that mm_malloc()
 *            and proper mm_free() calls won't crash.
 */

/*
 * mm_check - Check whether freeing the given `p`, which means that
 *            calling mm_free(p) leaves the heap in an ok state.
 */
int mm_can_free(void *p)
{
    int isMapped = ptr_is_mapped(p, GET_SIZE(HDRP(p)));

    if(isMapped && (GET_ALLOC(HDRP(p)) == YES))
        return 1;
    else
        return 0;
}

int mm_check()
{
    int i;
    void *bp = NULL;
    bp = startOfMemory;
    bp = FIRST_BP_IN_MEMORY(bp);

    for(i = 0; i < CHUNK_COUNT; i++)
    {
        if(ptr_is_mapped(bp, GET_SIZE(HDRP(bp))))
        {
          while (GET_SIZE(HDRP(bp)) != 0)
          {
              if(ptr_is_mapped(bp, GET_SIZE(HDRP(bp))))
              {
                if(GET_SIZE(HDRP(bp)) > 0 && GET_SIZE(FTRP(bp)) > 0)
                    bp = NEXT_BLKP(bp);
              }
              else
                  return 0;
          }

        }
        else
            return 0;
    }

  return 1;
}
