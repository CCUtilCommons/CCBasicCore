/**
 * @file memory_allocation.h
 * @author Charliechen114514 (chengh1922@mails.jlu.edu.cn)
 * @brief   This file is the center for the memory allocations,
 *          all memory allocation interface should go here to allocate
 *          the memory buffer instead of direct using malloc/free.
 * @version 0.1
 * @date 2025-09-11
 * @copyright Copyright (c) 2025
 *
 */

#ifndef CCBASICCORE_MEMALLOC_H
#define CCBASICCORE_MEMALLOC_H

#include <stddef.h>

/**
 * @brief   allocate_memory really allocate memory as the name goes,
 *          currently, the real allocations goes the
 *
 * @param buffer_size the size we wanna allocate buffer size
 * @return void* the buffer locations, if NULL, it means we allocate failed
 */
void* allocate_memory(size_t buffer_size);

/**
 * @brief free_memory
 *
 * @param buffer the buffer waiting
 * @return  int do we actually free off the buffer
 * @note    some interface might always returns yes, that means u should never
 *          consider the session :)
 */
int free_memory(void* buffer);

/**
 * @brief reallocate the buffer as we required
 *
 * @param buffer the buffer we wanna reallocate
 * @param new_size the new size we indicate
 * @return  void* the buffer we returns, to be cautions,
 *          buffer allocate previous may not be in the same
 *          start address, so, you can not discard the return value:)
 */
void* reallocate_memory(void* buffer, size_t new_size);

#endif
