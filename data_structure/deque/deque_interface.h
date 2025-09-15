#ifndef CCDEQUE_INTERFACE_H
#define CCDEQUE_INTERFACE_H
#include "deque_impl.h"
#include <stddef.h>
/**
 * @brief preanounced stack type
 *
 */
typedef struct CCDeque CCDeque;
/**
 * @brief interface type
 *
 */
typedef struct CCDeque_Interface CCDeque_Interface_t;

typedef void* CCDequeImpl;

typedef void* (*DequeInterface_CreateOp)(size_t);
typedef void (*DequeInterface_PushFrontOp)(CCDeque* deque, void* elem, size_t size);
typedef void* (*DequeInterface_PopFrontOp)(CCDeque* deque);
typedef void (*DequeInterface_PushBackOp)(CCDeque* deque, void* elem, size_t size);
typedef void* (*DequeInterface_PopBackOp)(CCDeque* deque);
typedef void* (*DequeInterface_FrontOp)(CCDeque* deque);
typedef void* (*DequeInterface_BackOp)(CCDeque* deque);
typedef size_t (*DequeInterface_SizeOp)(CCDeque* deque);
typedef int (*DequeInterface_EmptyOp)(CCDeque* deque);
typedef int (*DequeInterface_DestroyOp)(void* impl);

typedef struct CCDeque_Interface {
	DequeInterface_CreateOp create_ptr;
	DequeInterface_PushFrontOp pushfront_ptr;
	DequeInterface_PopFrontOp popfront_ptr;
	DequeInterface_PushBackOp pushback_ptr;
	DequeInterface_PopBackOp popback_ptr;
	DequeInterface_FrontOp front_ptr;
	DequeInterface_BackOp back_ptr;
	DequeInterface_SizeOp size_ptr;
	DequeInterface_EmptyOp empty_ptr;
	DequeInterface_DestroyOp destroy_ptr;
} CCDeque_Interface_t;

typedef struct CCDeque {
	CCDequeImpl deque_impl;
	CCDeque_Interface_t* interface;
	CCDequeImplCode_t code;
} CCDeque;

CCDeque* CCBasicCore_CCDequeCreate(
    size_t size, CCDeque_Interface_t* interface, CCDequeImplCode_t code);

int CCBasicCore_CCDequeDestroy(CCDeque* deque);

static inline void CCBasicCore_CCDequePushFront(CCDeque* deque, void* elem, size_t elem_size) {
	deque->interface->pushfront_ptr(deque, elem, elem_size);
}

static inline void* CCBasicCore_CCDequePopFront(CCDeque* deque) {
	return deque->interface->popfront_ptr(deque);
}

static inline void CCBasicCore_CCDequePushBack(CCDeque* deque, void* elem, size_t elem_size) {
	deque->interface->pushback_ptr(deque, elem, elem_size);
}

static inline void* CCBasicCore_CCDequePopBack(CCDeque* deque) {
	return deque->interface->popback_ptr(deque);
}

static inline void* CCBasicCore_CCDequeFront(CCDeque* deque) {
	return deque->interface->front_ptr(deque);
}

static inline size_t CCBasicCore_CCDequeSize(CCDeque* deque) {
	return deque->interface->size_ptr(deque);
}

static inline int CCBasicCore_CCDequeEmpty(CCDeque* deque) {
	return deque->interface->empty_ptr(deque);
}

#endif
