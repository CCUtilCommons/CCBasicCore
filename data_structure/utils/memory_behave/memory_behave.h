#ifndef CCMEMORY_BEHAVE_H
#define CCMEMORY_BEHAVE_H
#include "utils_def.h"
#include <stddef.h>

/**
 * @brief Memory Copy With Defined
 *
 * @param elem
 * @param elem_size
 * @param pack
 * @return void*
 */
void* ccmemory_copy(
    void* elem, size_t elem_size,
    CCSpecialDefinedPack* pack);

/**
 * @brief Release the Memery for the usage
 *
 * @param elem
 * @param pack
 * @return int
 */
int release_memory(void* elem, CCSpecialDefinedPack* pack);

#endif