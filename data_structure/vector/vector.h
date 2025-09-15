#ifndef CCBASIC_CORE_VECTOR_H
#define CCBASIC_CORE_VECTOR_H
#include "utils_def.h"
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
	CCSpecialDefinedPack* pack;
} CCVector;

/**
 * @brief create a blank vector
 *
 * @return CCVector* a blank vector
 */
CCVector* CCBasicCoreVector_CreateVector(size_t element_size); // signed as elem

CCVector* CCBasicCoreVector_CreateDefinedVector(size_t element_size, CCSpecialDefinedPack* pack);

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
 * @brief push front a element to a vector
 * @param vector the operating vector
 * @param element the element to be pushed
 * @param element_size the size of the element
 * @return int push front success?
 */
int __CCBasicCoreVector_PushFront(CCVector* vector,
                                  void* element,
                                  size_t element_size);

#define CCBasicCoreVector_PushFront(vector, element) \
	__CCBasicCoreVector_PushFront(vector, element, sizeof(*element))

/**
 * @brief vector.pop_front
 *
 * @param vector the operating vector
 * @return int pop front success?
 */
int CCBasicCoreVector_PopFront(CCVector* vector);

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
