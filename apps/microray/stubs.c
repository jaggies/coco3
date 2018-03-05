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

float floorf(float x) {
	return x;
}

float tan(float x) {
	return x;
}

float fabs(float x) {
	return x < 0.0f ? -x : x;
}

float pow(float x, float y) {
	return x*y;
}

float sqrt(float x) {
	return x/2;
}

float round(float x) {
	return (float) (int) x;
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

char* strdup(const char *s1) {
	return 0;
}

char* strndup(const char *s1, size_t n) {
	return 0;
}

int sscanf(const char* s, const char* format, ...) {
	return 0;
}

int scanf(const char* format, ...) {
	return 0;
}

