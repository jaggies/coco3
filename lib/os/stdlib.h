#ifndef _OS_STDLIB_H
#define _OS_STDLIB_H

#ifdef COCO
#include <cmoc.h>
extern void* malloc(int size);
#else
#include <stdlib.h>
#endif 

#endif // _OS_STDLIB_H
