#ifndef _OPENCL_BIG_ADD
#define _OPENCL_BIG_ADD
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include"simpleBigint.h"

SimpleBigint bigintAdd(SimpleBigint a1,SimpleBigint a2);
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

#endif // _LAYERS_MODEXP_OPENCL_
