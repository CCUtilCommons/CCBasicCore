#ifndef ERROR_H
#define ERROR_H

enum ErrorCode {
	CCBasicCore_SUCCESS,
	CCBasicCore_INDEX_OVERFLOW,
	CCBasicCore_INDEX_UNDERFLOW,
	CCBasicCore_MEMORY_ALLOCATION_FAILED,
	CCBasicCore_MEMORY_REALLOCATION_FAILED
};

/**
 * @brief 	When Wrong Overrides Occurs, you can invoke this for
 *			Your handlings
 *
 * @param who 	who suck the error
 * @param message the debug message
 */
void _FkOffTheWrongOverrider(void* who, const char* message);

/**
 * @brief When Wrong Types are happens to be here, handler is invoked!
 *
 * @param who 	who suck the error
 * @param message the debug message
 */
void _FkOffTheWrongType(void* who, const char* message);

#endif
