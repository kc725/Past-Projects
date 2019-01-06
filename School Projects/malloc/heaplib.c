#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

typedef struct _block_header {
    int block_size;
    bool free;
} block_info_t;

typedef struct _heap_header {
	int num_blocks;
} heap_info_t;

int padding(void *ptr, int space) {
	int diff = (uintptr_t)ADD_BYTES(ptr, space) % 8;
	if (diff == 0) {
		return 0;
	}
	return 8 - diff;
}

//moves the header by the amount of padding needed
void* move_block_header(block_info_t*block_header) {
	int pad = padding(block_header, sizeof(block_info_t));
	void *moveto = ADD_BYTES(block_header, pad);
	memmove(moveto, block_header, sizeof(block_info_t));
	return moveto;
}

bool coalesce(void *heap) {
	heap_info_t *heap_header = (heap_info_t*)heap;
	block_info_t *block_header = (block_info_t*)ADD_BYTES(heap, (int) sizeof(heap_info_t));
	int pad = padding(block_header, sizeof(block_info_t));
	block_header = (block_info_t*)ADD_BYTES(block_header, pad);
	void *start;
	void *next;
	int num_blocks_visited = 0;
	block_info_t *first;
	bool adjfirst = false;
	while (num_blocks_visited < heap_header->num_blocks) {
		start = ADD_BYTES(block_header, (int)sizeof(block_info_t));
		if (num_blocks_visited < heap_header->num_blocks - 1) {
			next = ADD_BYTES(start, block_header->block_size);
			next = ADD_BYTES(next, padding(next, sizeof(block_info_t)));
		}
		if (block_header->free == true) {
			if (adjfirst) {
				first->block_size += sizeof(block_info_t) + block_header->block_size;
				heap_header->num_blocks--;
				return true;
			}
			else {
				first = block_header;
				adjfirst = true;
			}
		}
		else {
			adjfirst = false;
		}
		num_blocks_visited++;
		block_header = (block_info_t*)next;
	}
	return false;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
  Might have to change what we're taking the sizeof() (class or the actual object).
  Add the padding to block_header to make sure that the address is 8-byte.
 */
void hl_init(void *heap, unsigned int heap_size) {
    
	heap_info_t *heap_header = (heap_info_t*)heap;
	heap_header->num_blocks = 1;
	block_info_t *block_header = (block_info_t*) ADD_BYTES(heap, sizeof(heap_info_t));
    block_header->free = true;
	block_header->block_size = heap_size - sizeof(block_info_t) - sizeof(heap_info_t);
	move_block_header(block_header);
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 * 
 * YOUR COMMENTS GO HERE.
	Make sure the pointer that is returned is 8-byte aligned by adding to it before returning.
	Always add padding to next's block header before returning so the size is accurate
 */
void *hl_alloc(void *heap, unsigned int block_size) {
	heap_info_t *heap_header = (heap_info_t*)heap;
	block_info_t *block_header = (block_info_t*)ADD_BYTES(heap, (int) sizeof(heap_info_t));
	int pad = padding(block_header, sizeof(block_info_t));
	block_header = (block_info_t*) ADD_BYTES(block_header, pad);
	void *start;
	void *next;
	int num_blocks_visited = 0;
	while (num_blocks_visited < heap_header->num_blocks) {
		start = ADD_BYTES(block_header, (int)sizeof(block_info_t));
		if (num_blocks_visited < heap_header->num_blocks - 1) {
			next = ADD_BYTES(start, block_header->block_size);
			next = ADD_BYTES(next, padding(next, sizeof(block_info_t)));
		}
		if (block_header->free == true && block_header->block_size >= block_size) {
			//allocate 
			int newblocksize = block_header->block_size - (block_size + sizeof(block_info_t) + padding(ADD_BYTES(start, block_size), sizeof(block_info_t)));
			if (newblocksize > 0) {
				block_info_t *newblock = (block_info_t*)ADD_BYTES(start, block_size);
				newblock->free = true;
				newblock->block_size = newblocksize;
				block_header->block_size = block_size + padding(newblock, sizeof(block_info_t));
				move_block_header(newblock);
				heap_header->num_blocks++;
			}
			block_header->free = false;	
			//coalesce(heap);
			return start;
		}
		block_header = (block_info_t*) next;
		num_blocks_visited++;
	} 
  
	//do nothing
	return 0;    
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 For optimization, combine nearby free blocks (recursive?), remember to reduce heap_header->num_blocks
 */
void hl_release(void *heap, void *block) {
	if (block != 0) {
		block_info_t *block_header = (block_info_t*) ADD_BYTES(block, -sizeof(block_info_t));
		block_header->free = true;
		coalesce(heap);
	}
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
	if (block == 0) {
		return hl_alloc(heap, new_size);
	}
	block_info_t *block_header = (block_info_t*)ADD_BYTES(block, -sizeof(block_info_t));
	block_header->free = true;
	void *ptr = hl_alloc(heap, new_size);
	if (ptr == 0) {
		block_header->free = false;
		return 0;
	}
	if (new_size < block_header->block_size) {
		memmove(ptr, block, new_size);
	}
	else {
		memmove(ptr, block, block_header->block_size);
	}
	return ptr;
}

