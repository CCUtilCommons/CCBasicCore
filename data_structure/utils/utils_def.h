#ifndef UTILS_DEF_H
#define UTILS_DEF_H
#include "compares.h"


typedef int (*SelfDefinedFree)(void* element);
typedef void* (*SelfDefinedCopy)(const void* elem);
typedef CompareResult (*SelfDefinedCompare)(const void* a, const void* b);

int CCCoreBasic_SimpleFreeBuffer(void* element);

static inline void* CCCoreBasic_SimpleCopyBuffer(const void* element) {
	return (void*)element; ///< thats what we want as simple copy
}

static inline CompareResult CCCoreBasic_SimpleCompareBuffer(
    const void* a, const void* b) {
	if (a == b) {
		return CCBASICCORE_COMPARERESULT_EQUAL;
	} else if (a < b) {
		return CCBASICCORE_COMPARERESULT_SMALLER;
	} else {
		return CCBASICCORE_COMPARERESULT_LARGE;
	}
}

/**
 * @brief CCSpecialDefinedPack defines the object copies, free and compares
 *
 */
typedef struct __SpecialDefinedPack {
	SelfDefinedCopy copier;
	SelfDefinedFree freer;
	SelfDefinedCompare compared;
} CCSpecialDefinedPack;

extern CCSpecialDefinedPack CCSpecialDefinedDefaultPack;

CCSpecialDefinedPack* CCBasicCoreCCSpecialDefinedPack_AssignedDefault(CCSpecialDefinedPack* pack);

#endif