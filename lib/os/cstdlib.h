/*
 * cstdlib.h
 *
 * A very simple stdlib implementation for allocators and FILE IO for Tandy CoCo.
 *
 *  Created on: Feb 3, 2019
 *
 *      Author: jmiller
 */

#ifndef _OS_CSTDLIB_H
#define _OS_CSTDLIB_H

#include <cmoc.h>

#define assert(cond) { }

struct FILE;

extern void* malloc(size_t size);
extern void free(void* ptr);
extern struct FILE * fopen(const char* filename, const char* mode);
extern int feof(struct FILE *fp);
extern char * fgets(char* str, int size, struct FILE * stream);
extern int fclose(struct FILE *stream);
extern int fputc(int c, struct FILE *stream);
extern int fprintf(struct FILE* stream, const char* format, ...);

#endif // _OS_CSTDLIB_H
