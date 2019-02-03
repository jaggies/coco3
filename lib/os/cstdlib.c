/*
 * cstdlib.c
 *
 * A very simple stdlib implementation for allocators and FILE IO for Tandy CoCo.
 *
 *  Created on: Feb 3, 2019
 *
 *      Author: jmiller
 */

#include "cstdlib.h"

typedef struct _Block {
    size_t size; // size of this block
    struct _Block* next;
} Block;

static size_t maxmem  = 0;
static size_t remaining = 0;
static Block* heap = 0;
static Block* freeList = 0;

void* malloc(size_t nbytes) {
    if (!heap) {
        maxmem = sbrkmax();
        heap = (Block*) sbrk(maxmem);
        heap->size = maxmem;
        heap->next = (Block*) 0;
        remaining = maxmem;
    }
    size_t chunkSize = nbytes + sizeof(Block);
    if (remaining >= chunkSize) {
        remaining -= chunkSize;
        Block* block = (Block*) heap;
        block->size = nbytes;
        block->next = (Block*) 0;
        heap += chunkSize;
        return (void*) ((char*) block + sizeof(Block));
    }
    return (void*) 0;
}

void free(void* ptr) {
    Block* block = (Block*) ((char*) ptr - sizeof(Block));
    block->next = freeList;
    freeList = block;
}

struct FILE * fopen(const char* filename, const char* mode) {
	return 0;
}

int feof(struct FILE *fp) {
	return 1;
}

char * fgets(char* str, int size, struct FILE * stream) {
	return 0;
}

int fclose(struct FILE *stream) {
	return 0;
}

int fputc(int c, struct FILE *stream) {
	return 0;
}

int fprintf(struct FILE* stream, const char* format, ...) {
	return 0;
}

