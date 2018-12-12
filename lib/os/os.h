#ifndef _OS_STDIO_H
#define _OS_STDIO_H

#ifdef COCO
#include <coco.h>
#include <cmoc.h>
struct FILE;
extern struct FILE * fopen(const char* filename, const char* mode);
extern int feof(struct FILE *fp);
extern char * fgets(char* str, int size, struct FILE * stream);
extern int fclose(struct FILE *stream);
extern int fputc(int c, struct FILE *stream);
extern int fprintf(struct FILE* stream, const char* format, ...);
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#endif // _OS_STDIO_H
