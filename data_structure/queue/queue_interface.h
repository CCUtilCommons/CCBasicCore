#ifndef CCQUEUE_INTERFACE_H
#define CCQUEUE_INTERFACE_H
#include "queue_impl.h"
#include <stddef.h>
/**
 * @brief preanounced stack type
 *
 */
typedef struct CCQueue CCQueue;
/**
 * @brief interface type
 *
 */
typedef struct CCQueue_Interface CCQueue_Interface_t;

typedef void* CCQueueImpl;

typedef void* (*QueueInterface_CreateOp)(size_t);
typedef void (*QueueInterface_PushOp)(CCQueue* queue, void* elem, size_t size);
typedef void* (*QueueInterface_PopOp)(CCQueue* queue);
typedef void* (*QueueInterface_FrontOp)(CCQueue* queue);
typedef size_t (*QueueInterface_SizeOp)(CCQueue* queue);
typedef int (*QueueInterface_EmptyOp)(CCQueue* queue);
typedef int (*QueueInterface_DestroyOp)(void* impl);

typedef struct CCQueue_Interface {
	QueueInterface_CreateOp create_ptr;
	QueueInterface_PushOp push_ptr;
	QueueInterface_PopOp pop_ptr;
	QueueInterface_FrontOp front_ptr;
	QueueInterface_SizeOp size_ptr;
	QueueInterface_EmptyOp empty_ptr;
	QueueInterface_DestroyOp destroy_ptr;
} CCQueue_Interface_t;

typedef struct CCQueue {
	CCQueueImpl queue_impl;
	CCQueue_Interface_t* interface;
	CCQueueImplCode_t code;
} CCQueue;

CCQueue* CCBasicCore_CCQueueCreate(
    size_t size, CCQueue_Interface_t* interface, CCQueueImplCode_t code);

int CCBasicCore_CCQueueDestroy(CCQueue* queue);

static inline void CCBasicCore_CCQueuePush(CCQueue* queue, void* elem, size_t elem_size) {
	queue->interface->push_ptr(queue, elem, elem_size);
}

static inline void* CCBasicCore_CCQueuePop(CCQueue* queue) {
	return queue->interface->pop_ptr(queue);
}

static inline void* CCBasicCore_CCQueueFront(CCQueue* queue) {
	return queue->interface->front_ptr(queue);
}

static inline size_t CCBasicCore_CCQueueSize(CCQueue* queue) {
	return queue->interface->size_ptr(queue);
}

static inline int CCBasicCore_CCQueueEmpty(CCQueue* queue) {
	return queue->interface->empty_ptr(queue);
}

#endif
