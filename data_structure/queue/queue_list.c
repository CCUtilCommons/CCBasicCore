#include "queue_list.h"
#include "Error.h"
#include "list.h"
#include "memory_allocation.h"
#include "queue_impl.h"
#include "queue_interface.h"
#include <stddef.h>
static const char* _wrongoverride_list_message = "Wrong Impl Error happens in CCList queue";

static void* _createListPtr(size_t elem_size) {
	return CCBasicCore_initCCList(elem_size);
}

static void _pushListPPtr(
    CCQueue* queue, void* elem, size_t size) {
	if (queue->code != CCQUEUE_LIST_IMPL) {
		_FkOffTheWrongOverrider(queue,
		                        _wrongoverride_list_message);
		return; // Not a valid code, or things got wrong
	}

	// Pass the Impl Check
	CCList* list = (CCList*)queue->queue_impl;

	if (CCBasicCore_SUCCESS != CCBasicCore_CCListPushBack(list, elem, size)) {
		_FkOffTheWrongType(queue, "Error in Handling the Size Session");
		return;
	}
}

static void* _topListPPtr(CCQueue* queue) {
	if (queue->code != CCQUEUE_LIST_IMPL) {
		_FkOffTheWrongOverrider(queue,
		                        _wrongoverride_list_message);
		return NULL; // Not a valid code, or things got wrong
	}
	// Pass the Impl Check
	CCList* list = (CCList*)queue->queue_impl;

	struct CCListNode* node = CCBasicCore_CCListHead(list);
	if (!node)
		return NULL;
	return CCBasicCore_GetListNode(node);
}

static void* _popListPPtr(CCQueue* queue) {
	if (queue->code != CCQUEUE_LIST_IMPL) {
		_FkOffTheWrongOverrider(queue,
		                        _wrongoverride_list_message);
		return NULL; // Not a valid code, or things got wrong
	}
	CCList* list = (CCList*)queue->queue_impl;
	return CCBasicCore_CCListPopFront(list);
}

static size_t _sizePtr(CCQueue* queue) {
	if (queue->code != CCQUEUE_LIST_IMPL) {
		_FkOffTheWrongOverrider(queue,
		                        _wrongoverride_list_message);
		return 0; // Not a valid code, or things got wrong
	}
	// Pass the Impl Check
	CCList* list = (CCList*)queue->queue_impl;
	return CCBasicCore_CCListSize(list);
}

static int _emptyPtr(CCQueue* queue) {
	if (queue->code != CCQUEUE_LIST_IMPL) {
		_FkOffTheWrongOverrider(queue,
		                        _wrongoverride_list_message);
		return 0; // Not a valid code, or things got wrong
	}
	// Pass the Impl Check
	CCList* list = (CCList*)queue->queue_impl;
	return CCBasicCore_CCListEmpty(list);
}

static int _destroyPtr(void* elem) {
	CCList* list = (CCList*)elem;
	return CCBasicCore_destroyCCList(list);
}

static CCQueue_Interface_t listQueue_t = {
	_createListPtr,
	_pushListPPtr,
	_popListPPtr,
	_topListPPtr,
	_sizePtr,
	_emptyPtr,
	_destroyPtr
};

CCQueue* CCBasicCore_CCListQueueCreate(size_t size) {
	return CCBasicCore_CCQueueCreate(size, &listQueue_t, CCQUEUE_LIST_IMPL);
}
