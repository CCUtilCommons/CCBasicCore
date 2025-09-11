#include <stddef.h>
#include <stdlib.h>



/**
 * @brief   allocate_memory really allocate memory as the name goes,
 *          currently, the real allocations goes the
 *
 * @param buffer_size the size we wanna allocate buffer size
 * @return void* the buffer locations, if NULL, it means we allocate failed
 */
void* allocate_memory(size_t buffer_size){
    return malloc(buffer_size);
}

/**
 * @brief free_memory
 *
 * @param buffer the buffer waiting
 * @return  int do we actually free off the buffer
 * @note    some interface might always returns yes, that means u should never
 *          consider the session :)
 */
int free_memory(void* buffer){
    free(buffer);
    return 1;   ///< Always success yet
}

/**
 * @brief reallocate the buffer as we required
 *
 * @param buffer the buffer we wanna reallocate
 * @param new_size the new size we indicate
 * @return  void* the buffer we returns, to be cautions,
 *          buffer allocate previous may not be in the same
 *          start address, so, you can not discard the return value:)
 */
void* reallocate_memory(void* buffer, size_t new_size){
    return realloc(buffer, new_size);
}
