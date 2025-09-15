#include "queue_impl.h"
#include "Error.h"
#include "queue_interface.h"
#include <stddef.h>
#include <string.h>
#include "utils_def.h"
#include "vector.h"
#include "queue_vector.h"
#include "result_pack.h"
#include "memory_allocation.h"

static const char* _wrongoverride_vector_message = 
    "Wrong Impl Error happens in CCVector Queue";


static void* _createPtr(size_t size) {
     return CCBasicCoreVector_CreateVector(size);
}

static int _destroyPtr(void* elem){
    CCVector* vector = (CCVector*)elem;
    return CCBasicCoreVector_FreeVector(vector);
}

static void _pushVectorPtr(
    CCQueue* queue, void* elem, size_t size){
    if(queue->code != CCQUEUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->queue_impl;
    if(!__CCBasicCoreVector_PushBack(vector, elem, size)){
        _FkOffTheWrongType(queue, "Error in Handling the Size Session");
        return;
    }
}

static void* _topVectorPtr(CCQueue* queue){
    if(queue->code != CCQUEUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->queue_impl;
    size_t size = vector->current_size;
    if(size == 0){
        return NULL; // OK we returns nothing
    }

    ResultPack* elem = CCBasicCoreVector_Get(vector, 0);
    if(elem->status_code != CCBasicCore_SUCCESS){
        return NULL;
    }

    return elem->result;
}

static void* _popVectorPtr(CCQueue* queue){
    if(queue->code != CCQUEUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    void* elem = _topVectorPtr(queue);
    CCVector* vector = (CCVector*)queue->queue_impl;
    void* result = NULL;
    if(elem){
        if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(vector->pack)){
            result = vector->pack->copier(elem);
        }else{
            result = allocate_memory(vector->element_size);
            memcpy(result, elem, vector->element_size);
        } 
    }
    CCBasicCoreVector_PopFront(vector);
    return result;
}

static size_t _sizePtr(CCQueue* queue){
    if(queue->code != CCQUEUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->queue_impl;
    return vector->current_size;
}

static int _emptyPtr(CCQueue* queue){
    if(queue->code != CCQUEUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->queue_impl;
    return vector->current_size == 0;
}


static CCQueue_Interface_t vectorQueue_t = {
    _createPtr,
    _pushVectorPtr,
    _popVectorPtr,
    _topVectorPtr,
    _sizePtr,
    _emptyPtr,
    _destroyPtr
};

CCQueue* CCBasicCore_CCVectorQueueCreate(size_t size){
    return CCBasicCore_CCQueueCreate(size, &vectorQueue_t, CCQUEUE_VECTOR_IMPL);
}