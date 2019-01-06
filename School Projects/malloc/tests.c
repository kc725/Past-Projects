#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"
#include <stdint.h>

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* our SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* your SPEC tests */
    /* 4  */ "alloc HEAP_SIZE/2, try to alloc another HEAP_SIZE/2 which shouldn't work, free, then try to alloc again, should pass",
    /* 5  */ "alloc, realloc to smaller size, then realloc to a size larger than the heap, should fail",
    /* 6  */ "alloc to divide the entire heap into certain block sizes, then free and alloc to those different blocks to test that free makes them available again",
    /* 7  */ "freeing a block of size 0 acts as NOP (NULL)",
    /* 8  */ "alloc non-multiples of 8, check that the return addresses are 8-byte aligned",
    /* 9  */ "alloc almost the entire heap, then alloc size 0 again, should pass",
    /* 10 */ "compare alloc with a realloc of a block of size 0, should pass",
    /* 11 */ "try realloc a block with NULL pointer, should function the same as alloc",
    /* 12 */ "realloc to different sizes (larger and smaller), check content is preserved",
    /* 13 */ "test that alloc and realloc is putting blocks within the heap, even with different heap sizes",
    /* 14 */ "always resizes to contiguous block of memory",
    /* 15 */ "your description here",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* 17 */ "fill the heap with as many small blocks possible",
    /* 18 */ "alloc, free, & realloc, stay within heap limits but also check outside heap limits",
    /* 19 */ "alloc and realloc within heap limits, check data integrity",
    /* 20 */ "test various reallocs to make sure the blocks remain contiguous even when there are other blocks present",
    /* 21 */ "alloc HEAP_SIZE/2 and try again, free then try again, for increasing HEAP_SIZE",
    /* 22 */ "your description here",
    /* 23 */ "your description here",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns null, test01 returns FAILURE (==0)
    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test02 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init, alloc, and release
 * SPECIFICATION BEING TESTED:
 * release should free up space in the heap for alloc.
 *
 * MANIFESTATION OF ERROR:
 * alloc will return a pointer if it succeeds or 0. By filling with blocks of HEAP_SIZE/2, we
 * make sure there can not be two blocks in the heap (metadata takes up space) so we can test if
 * alloc fails and release succeeds.
 */
int test04() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr = hl_alloc(heap, HEAP_SIZE / 2);
	if (hl_alloc(heap, HEAP_SIZE / 2) != 0) {
		return FAILURE;
	}
	hl_release(heap, ptr);
    return hl_alloc(heap, HEAP_SIZE / 2) > 0;
}


    


/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * resize works if new_size < HEAP_SIZE but fails if new_size >= HEAP_SIZE
 *
 * MANIFESTATION OF ERROR:
 * If resizing to a smaller size fails or if resizing to a larger size works.
 */
int test05() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	char* ptr1 = hl_alloc(heap, 4*sizeof(char));
	*ptr1 = 'a';
	ptr1[1] = 'b';
	ptr1[2] = 'c';
	ptr1[3] = 'd';
	ptr1 = hl_resize(heap, ptr1, 2 * sizeof(char));
	char *ptr2 = hl_alloc(heap, 2 * sizeof(char));
	*ptr2 = 'e';
	ptr2[1] = 'f';
	return *ptr1 == 'a' && ptr1[1] == 'b' && *ptr2 == 'e' && ptr2[1] == 'f';
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc and release
 * SPECIFICATION BEING TESTED:
 * alloc does not allocate non-contiguous blocks of memory
 *
 * MANIFESTATION OF ERROR:
 * We begin by allocating various blocks to fill up the heap. Then we free two of those blocks
 * and try to allocate a block with the size of their sum. If alloc allocates non-continguous 
 * blocks of memory, then it will succeed.
 */
int test06() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr1 = hl_alloc(heap, HEAP_SIZE / 6);
	hl_alloc(heap, HEAP_SIZE / 6);
	int *ptr3 = hl_alloc(heap, HEAP_SIZE / 6);
	hl_alloc(heap, HEAP_SIZE / 6);
	hl_release(heap, ptr1);
	hl_release(heap, ptr3);
	return hl_alloc(heap, HEAP_SIZE / 3) == 0;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: release
 * SPECIFICATION BEING TESTED:
 * release of a block of size 0 should act as a NOP
 *
 * MANIFESTATION OF ERROR:
 * If release can not handle size 0, it will throw an exception. It should also not change any data.
 */
int test07() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr1 = hl_alloc(heap, 4);
	*ptr1 = 5;
	char *ptr2 = hl_alloc(heap, 1);
	*ptr2 = 'a';
	hl_release(heap, NULL);
    return *ptr1 == 5 && *ptr2 == 'a';
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc
 * SPECIFICATION BEING TESTED:
 * The address is 8-byte aligned.
 *
 * MANIFESTATION OF ERROR:
 * The address is not divisible by 8. 
 */
int test08() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	for (int i = 1; i <= 7; i++) {
		int *ptr = hl_alloc(heap, i);
		if ((uintptr_t)ptr % 8 != 0) {
			return FAILURE;
		}
		ptr = hl_resize(heap, ptr, i + 8);
		if((uintptr_t)ptr % 8 != 0) {
			return FAILURE;
		}
	}
	
    return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc
 * SPECIFICATION BEING TESTED:
 * It is possible to alloc a block of size 0 and that does not affect the data in the heap.
 *
 * MANIFESTATION OF ERROR:
 * alloc of size 0 will return SEGFAULT if not possible, or the data in the heap is changed.
 */
int test09() {
	char heap[HEAP_SIZE];
	for (int i = 1; i < 8; i++) {
		hl_init(&heap[i], HEAP_SIZE - i);
		char* ptr = hl_alloc(&heap[i], HEAP_SIZE / 2);
		if (ptr < &heap[i] || (uintptr_t)ptr % 8 != 0) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * realloc a block between 2 blocks and ensure that the next block doesn't get overwritten
 *
 * MANIFESTATION OF ERROR:
 * If the data isn't equal, then returns failure
 */
int test10() {
	
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr = hl_alloc(heap, HEAP_SIZE / 4);
	*ptr = 'a';
	for (int i = HEAP_SIZE / 4; i > sizeof(int); i--) {
		ptr = hl_resize(heap, ptr, i);
		if (*ptr != 'a') {
			return FAILURE;
		}
	}
	return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: realloc
 * SPECIFICATION BEING TESTED:
 * realloc to a NULL pointer should act as an alloc
 *
 * MANIFESTATION OF ERROR:
 * SEGFAULT or no alloc occured 
 */
int test11() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr = hl_resize(heap, NULL, 4);
	*ptr = 6;
	return *ptr == 6;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc and resize
 * SPECIFICATION BEING TESTED:
 * Data is maintained in the heap even after various allocs and resizes.
 *
 * MANIFESTATION OF ERROR:
 * The data is somehow altered.
 */
int test12() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr1 = hl_alloc(heap, sizeof(int));
	*ptr1 = 5;
	char *ptr2 = hl_alloc(heap, sizeof(char));
	*ptr2 = 'a';
	int *ptr3 = hl_alloc(heap, sizeof(int));
	*ptr3 = 3;
	if (*ptr3 == 3) {
		if (*ptr2 == 'a') {
			hl_resize(heap, ptr1, sizeof(int) + 1);
			return *ptr1 == 5;
		}
	}
    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc, free and realloc
 * SPECIFICATION BEING TESTED: 
 * alloc and realloc make sure the block stays within the heap. Free should also be able to work.
 * MANIFESTATION OF ERROR: 
 * the pointer to the block lies outside the heap or SEGFAULT
 */
int test13() {
	int n_tries = 50;
	int block_size = 16;

	char memarea[HEAP_SIZE * 3];

	char *heap_start = &memarea[1024];
	char *pointers[NPOINTERS];

	memset(pointers, 0, NPOINTERS * sizeof(char *));

	hl_init(heap_start, HEAP_SIZE);
	srandom(0);
	for (int i = 0; i < n_tries; i++) {
		int index = random() % NPOINTERS;
		if (pointers[index] == 0) {
			pointers[index] = hl_alloc(heap_start, block_size);
			if ((uintptr_t)pointers[index] % 8 != 0) {
				return FAILURE;
			}
			if (pointers[index] != 0 && (pointers[index] < &memarea[1024] || pointers[index] > &memarea[2048])) {
				return FAILURE;
			}
		}
		else {
			if (rand() % 1 == 0) {
				hl_release(heap_start, pointers[index]);
				pointers[index] = 0;
			}
			else {
				pointers[index] = hl_resize(heap_start, pointers[index], rand() % HEAP_SIZE);
			}
		}
	}
	return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc and realloc
 * SPECIFICATION BEING TESTED:
 * alloc and realloc only use contiguous blocks
 *
 * MANIFESTATION OF ERROR:
 * Any of the allocs or reallocs into non-contiguous blocks work 
 */
int test14() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr1 = hl_alloc(heap, HEAP_SIZE / 8);
	hl_alloc(heap, HEAP_SIZE / 8);
	int *ptr3 = hl_alloc(heap, HEAP_SIZE / 8);
	hl_alloc(heap, HEAP_SIZE / 8);
	int *ptr5 = hl_alloc(heap, HEAP_SIZE / 8);
	hl_alloc(heap, HEAP_SIZE / 8);
	hl_release(heap, ptr3);
	hl_release(heap, ptr5);
	return hl_resize(heap, ptr1, HEAP_SIZE / 4) == 0;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * init works for different heap sizes
 *
 * MANIFESTATION OF ERROR:
 * segfault or alloc, resize, or release fail
 */
int test15() {
	char heap[HEAP_SIZE * 5];
	for (int i = 1; i < 5; i++) {
		hl_init(heap, HEAP_SIZE*i);
		int *ptr = hl_alloc(heap, sizeof(int));
		*ptr = 4;
		ptr = hl_resize(heap, ptr, HEAP_SIZE / 2);
		if (*ptr != 4) {
			return FAILURE;
		}
		hl_release(heap, ptr);
	}
	return SUCCESS;
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {

    char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int size = 16;
    int *ptr = hl_alloc(heap,size);
    *ptr = 'a';

	while (ptr != 0) {
        if (*ptr != 'a') {
			return FAILURE;
		}
       ptr = hl_resize(heap, ptr, ++size);
    }
    
    return SUCCESS;
    
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED alloc
 * INTEGRITY OR DATA CORRUPTION?
 * Both.
 * MANIFESTATION OF ERROR:
 * A SEGFAULT or corrupted data or writing outside the heap.
 */
int test17() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	char *pointers[10];
	for (int i = 0; i <= (((HEAP_SIZE / 2) - 28) / 8); i+= 10) {
		memset(pointers, 0, 10);
		for (int j = 0; j < 10; j++) {
			pointers[j] = hl_alloc(heap, j + i);
			if ((uintptr_t)pointers[j] % 8 != 0) {
				return FAILURE;
			}
		}
		for (int j = 0; j < 10; j++) {
			hl_release(heap, pointers[j]);
		}
	}
	return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, free, and realloc
 * INTEGRITY OR DATA CORRUPTION?
 * integrity
 * MANIFESTATION OF ERROR:
 * SEGFAULT or outside heap limits. Also checks that addresses are 8-byte aligned.
 */
int test18() {
	int n_tries = 100000;
	int block_size = 16;

	char memarea[HEAP_SIZE * 3];

	char *heap_start = &memarea[1024];
	char *pointers[NPOINTERS];

	memset(pointers, 0, NPOINTERS * sizeof(char *));

	hl_init(heap_start, HEAP_SIZE);
	srandom(0);
	for (int i = 0; i < n_tries; i++) {
		int index = random() % NPOINTERS;
		if (pointers[index] == 0) {
			pointers[index] = hl_alloc(heap_start, block_size);
			if ((uintptr_t)pointers[index] % 8 != 0) {
				return FAILURE;
			}
			if (pointers[index] != 0 && (pointers[index] < &memarea[1024] || pointers[index] > &memarea[2048])) {
				return FAILURE;
			}
		}
		else {
			if (rand() % 1 == 0) {
				hl_release(heap_start, pointers[index]);
				pointers[index] = 0;
			}
			else {
				int randsize = rand() % HEAP_SIZE;
				pointers[index] = hl_resize(heap_start, pointers[index], randsize);
			}
		}
	}
	return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: realloc
 * INTEGRITY OR DATA CORRUPTION?
 * data corruption
 * MANIFESTATION OF ERROR:
 * the value is changed
 */
int test19() {
	char heap[HEAP_SIZE];
	hl_init(heap, HEAP_SIZE);
	int *ptr = hl_alloc(heap, HEAP_SIZE / 4);
	*ptr = 'a';
	for (int i = HEAP_SIZE / 4; i > sizeof(int); i--) {
		ptr = hl_resize(heap, ptr, i);
		if (*ptr != 'a') {
			return FAILURE;
		}
	}
	/*for (int j = HEAP_SIZE / 4; j < HEAP_SIZE / 2; j++) {
		ptr = hl_resize(heap, ptr, j);
		if (*ptr != 'a') {
			return FAILURE;
		}
	}*/
	return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: realloc
 * INTEGRITY OR DATA CORRUPTION?
 * integrity
 * MANIFESTATION OF ERROR:
 * realloc succeeds in returning a block when there is not enough space.
 */
int test20() {
	for (int n = 8; n < HEAP_SIZE/2; n += 2) {
		char heap[HEAP_SIZE];
		hl_init(heap, HEAP_SIZE);
		int *ptr1 = hl_alloc(heap, HEAP_SIZE / n);
		hl_alloc(heap, HEAP_SIZE / n);
		int *ptr3 = hl_alloc(heap, HEAP_SIZE / n);
		hl_alloc(heap, HEAP_SIZE / n);
		int *ptr5 = hl_alloc(heap, HEAP_SIZE / n);
		hl_alloc(heap, HEAP_SIZE / n);
		hl_release(heap, ptr3);
		hl_release(heap, ptr5);
		if (hl_resize(heap, ptr1, HEAP_SIZE - HEAP_SIZE * 6 / n) != 0) {
			return FAILURE;
		}
	}
	return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc and free
 * INTEGRITY OR DATA CORRUPTION?
 * integrity
 * MANIFESTATION OF ERROR:
 * alloc works when there is no space or free does not release a block
 */
int test21() {
	for (int i = 1; i < 1000; i++) {
		char heap[1000*HEAP_SIZE];
		hl_init(heap, i*HEAP_SIZE);
		int *ptr = hl_alloc(heap, i * HEAP_SIZE / 2);
		if (hl_alloc(heap, i * HEAP_SIZE / 2) != 0) {
			return FAILURE;
		}
		hl_release(heap, ptr);
		if (hl_alloc(heap, i * HEAP_SIZE / 2) == 0) {
			return FAILURE;
		}
	}
	return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {
	for (int j = 1; j < 1000; j++) {
		char heap[1000*HEAP_SIZE];
		for (int i = 1; i < 8; i++) {
			hl_init(&heap[i], HEAP_SIZE - i);
			char* ptr = hl_alloc(&heap[i], HEAP_SIZE / 4);
			if (ptr < &heap[i] || (uintptr_t)ptr % 8 != 0) {
				return FAILURE;
			}
			*ptr = 'a';
            ptr[1] = 'a';
            ptr[2] = 'a';
			char *ptr2 = hl_alloc(&heap[i], HEAP_SIZE / 4);
			if (ptr2 < &heap[i] || (uintptr_t)ptr2 % 8 != 0) {
				return FAILURE;
			}
			hl_resize(&heap[i], ptr, HEAP_SIZE/3);
            hl_alloc(&heap[i], HEAP_SIZE/512);
			hl_release(&heap[i], ptr2);
			if (*ptr != 'a') {
				return FAILURE;
			}
        
		}
	}
	return SUCCESS;
}



/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_init
 * INTEGRITY OR DATA CORRUPTION?
 * integrity and data corruption
 * MANIFESTATION OF ERROR:
 * hl_init of different heap sizes causes a SEGFAULT or data corruption
 */
int test23() {
	char heap[HEAP_SIZE * 1000];
	for (int i = 1; i < 1000; i++) {
		hl_init(heap, HEAP_SIZE*i);
		int *ptr = hl_alloc(heap, sizeof(int));
		*ptr = 4;
		ptr = hl_resize(heap, ptr, HEAP_SIZE / 2);
		if (*ptr != 4) {
			return FAILURE;
		}
		hl_release(heap, ptr);
	}
	return SUCCESS;
}