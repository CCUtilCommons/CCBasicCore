#ifndef CCSTACK_VECTOR_H
#define CCSTACK_VECTOR_H
#include <stddef.h>

typedef struct CCStack CCStack;
CCStack* CCBasicCore_CreateVectorStack(size_t elem_size);

#endif