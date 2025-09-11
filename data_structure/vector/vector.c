#include "vector.h"
#include <stddef.h>
#include <string.h>
#include "result_pack.h"
#include "Error.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
static int __shell_resize(CCVector* vector){
    return vector->capacity == vector->current_size || // Overflow
           (vector->current_size > 0 && vector->capacity >= 4 * vector->current_size); // Underflow
}

static int __reallocate_memory(CCVector* vector, size_t new_size){
    void* new_addr = reallocate_memory(vector->buffer_container, 
            new_size * vector->element_size);
    if(!new_addr){
        return 0;   // We failed to allocate memory :(
    }

    vector->buffer_container = new_addr;
    vector->capacity = new_size;

    // adjust size
    if(new_size < vector->current_size) {
        vector->current_size = new_size;
    }

    
    return 1;
}


/**
 * @brief create a blank vector
 *
 * @return CCVector* a blank vector
 */
CCVector* CCBasicCoreVector_CreateVector(size_t element_size){
    CCVector* vector = allocate_one(CCVector);
    vector->capacity = 1;
    vector->current_size = 0;
    vector->element_size = element_size;
    vector->buffer_container = allocate_memory(vector->capacity * element_size);;

    return vector;
}

/**
 * @brief free a vector
 *
 * @param vector the vector we gonna free
 * @return int if we free success
 */
int CCBasicCoreVector_FreeVector(CCVector* vector){
    // free we holdings
    int result = free_memory(vector->buffer_container);
    
    // free itself
    result &= free_memory(vector);
    
    // throw these back 
    return result;
}

/**
 * @brief resize the given vector
 *
 * @param vector
 * @param new_size
 * @return int indicate if we resize success
 */
int CCBasicCoreVector_ResizeVector(CCVector* vector, size_t new_size)
{
    vector->capacity = new_size;
    return  __reallocate_memory(vector, new_size);
}

/**
 * @brief push back a element to a vector
 *
 */
int __CCBasicCoreVector_PushBack(CCVector* vector,
                                 void* element,
                                 size_t element_size){
    // Type mismatch
    if(element_size != vector->element_size){
        return 0;
    }

    if(__shell_resize(vector)){
       __reallocate_memory(vector, vector->capacity * 2);
    }                                
    
    // Assigned!
    int result = __CCBasicCoreVector_Set(vector, vector->current_size, element, element_size);
    vector->current_size++;
    return result;
}


/**
 * @brief vector.pop_back
 *
 * @param vector the operating vector
 * @return int pop back success?
 */
int CCBasicCoreVector_PopBack(CCVector* vector){
    if(__shell_resize(vector)){
        __reallocate_memory(vector, vector->capacity / 2);
    }     
    vector->current_size--;
    return 1;
}

/**
 * @brief visit the array
 *
 * @param vector
 * @param index
 * @return struct ResultPack* for the possible get
 */
struct ResultPack* CCBasicCoreVector_Get(CCVector* vector, size_t index)
{
    if(vector->current_size <= index)
        return CCBasicCoreResultPack_AssignedResultPack(
            NULL, CCBasicCore_INDEX_OVERFLOW);

    return CCBasicCoreResultPack_AssignedResultPack(
        get_operating_offset(vector->buffer_container, vector->element_size, index)
        , CCBasicCore_SUCCESS);
}

/**
 * @brief __setter
 *
 * @param vector
 * @param index
 * @param element
 * @param element_size
 * @return int
 */
int __CCBasicCoreVector_Set(CCVector* vector, size_t index, void* element,
                            size_t element_size)
{
    if(vector->element_size != element_size)
        return 0;

    if(vector->current_size <= index)
        return 0;

    char* written_addr = get_operating_offset(vector->buffer_container, element_size, index);
    memcpy(written_addr, element, element_size);

    return 1;
}





