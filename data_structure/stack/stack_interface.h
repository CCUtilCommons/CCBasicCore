#ifndef CCSTACK_INTERFACE_H
#define CCSTACK_INTERFACE_H
#include "stack_impl.h"
#include <stddef.h>
/**
 * @brief preanounced stack type
 *
 */
typedef struct CCStack CCStack;
/**
 * @brief interface type
 *
 */
typedef struct CCStackInterface CCStackInterface_t;

typedef void* CCStackImpl;

/**
 * @brief Interface of Create Ptr
 *          params: element_op size
 *          returns: impl types or containers
 *
 */
typedef void* (*StackInterface_CreateOp)(size_t);

typedef void* (*StackInterface_PopOp)(CCStack*);
typedef void* (*StackInterface_TopOp)(CCStack*);
typedef void (*StackInterface_PushOp)(CCStack*, void*, size_t);
typedef int (*StackInterface_EmptyOp)(CCStack*);
typedef size_t (*StackInterface_SizeOp)(CCStack*);
typedef int (*StackInterface_DestroyOp)(void* impl);

typedef struct CCStackInterface {
	StackInterface_CreateOp creator_ptr;
	StackInterface_PushOp push_ptr;
	StackInterface_PopOp pop_ptr;
	StackInterface_TopOp top_ptr;
	StackInterface_EmptyOp empty_ptr;
	StackInterface_SizeOp size_ptr;
	StackInterface_DestroyOp destroy_op;
} CCStackInterface_t; // Stack Interface Required

typedef struct CCStack {
	CCStackInterface_t* interfaces; ///< stack operate interfaces
	CCStackImpl impl; ///< implements op
	CCStackImplCode_t code; ///< code type
} CCStack;

CCStack* CCBasicCore_CCStackCreate(
    size_t elem_size, CCStackImplCode_t impl_code, CCStackInterface_t* interfaces);

static inline void CCBasicCore_CCStackPush(
    CCStack* stack,
    void* elem, size_t elem_size) {
	// repost to the interface
	stack->interfaces->push_ptr(stack, elem, elem_size);
}

static inline void* CCBasicCore_CCStackPop(
    CCStack* stack) {
	return stack->interfaces->pop_ptr(stack);
}

static inline void* CCBasicCore_CCStackTop(CCStack* stack) {
	return stack->interfaces->top_ptr(stack);
}

static inline int CCBasicCore_CCStackEmpty(CCStack* stack) {
	return stack->interfaces->empty_ptr(stack);
}

static inline size_t CCBasicCore_CCStackSize(CCStack* stack) {
	return stack->interfaces->size_ptr(stack);
}

int CCBasicCore_CCStackDestroy(CCStack* stack);

#endif
