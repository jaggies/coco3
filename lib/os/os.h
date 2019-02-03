#ifndef _OS_STDIO_H
#define _OS_STDIO_H

#define Number(a) (sizeof(a) / sizeof(a[0]))

#ifdef COCO
#include <coco.h>
#include <cmoc.h>
#include "cstdlib.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#endif

#endif // _OS_STDIO_H
