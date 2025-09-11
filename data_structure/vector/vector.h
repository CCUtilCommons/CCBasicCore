#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct ResultPack; ///< see utils for includes

/**
 * @brief   CCVector is a dynamic array working for dynamic
 *          issue
 *
 */
typedef struct CCVector {
	size_t capacity;
	size_t current_size;
	size_t element_size;
	void* buffer_container;
} CCVector;

/**
 * @brief create a blank vector
 *
 * @return CCVector* a blank vector
 */
CCVector* CCBasicCoreVector_CreateVector(size_t element_size); // signed as elem

/**
 * @brief free a vector
 *
 * @param vector the vector we gonna free
 * @return int if we free success
 */
int CCBasicCoreVector_FreeVector(CCVector* vector);

/**
 * @brief resize the given vector
 *
 * @param vector
 * @param new_size
 * @return int indicate if we resize success
 */
int CCBasicCoreVector_ResizeVector(CCVector* vector, size_t new_size);

/**
 * @brief push back a element to a vector
 *
 */
int __CCBasicCoreVector_PushBack(CCVector* vector,
                                 void* element,
                                 size_t element_size);

/**
 * @brief   CCBasicCoreVector_PushBack is the recommend interface for the
 *          calling
 *
 */
#define CCBasicCoreVector_PushBack(vector, element) \
	__CCBasicCoreVector_PushBack(vector, element, sizeof(*element))

/**
 * @brief vector.pop_back
 *
 * @param vector the operating vector
 * @return int pop back success?
 */
int CCBasicCoreVector_PopBack(CCVector* vector);

/**
 * @brief visit the array
 *
 * @param vector
 * @param index
 * @return struct ResultPack* for the possible get
 */
struct ResultPack* CCBasicCoreVector_Get(CCVector* vector, size_t index);

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
                            size_t element_size);

#define CCBasicCoreVector_Set(vector, index, element) \
	__CCBasicCoreVector_Set(vector, index, element, sizeof(*element))

#endif
