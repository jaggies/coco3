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
#include <string.h>
#include <math.h>
#define interrupt
#endif

#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

#endif // _OS_STDIO_H
