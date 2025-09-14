#ifndef CCSTACK_LIST_H
#define CCSTACK_LIST_H
#include <stddef.h>

typedef struct CCStack CCStack;
CCStack* CCBasicCore_CreateListStack(size_t elem_size);

#endif