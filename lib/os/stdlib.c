#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEM 16384

static char mem[MAX_MEM];
static int allocated  = 0;

void* malloc(int size) {
	if ((size+allocated) < MAX_MEM) {
		allocated += size;
		return (void*) &mem[allocated];
	}
	return 0; // no more!
}

void free(void*) {
	// TODO
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

