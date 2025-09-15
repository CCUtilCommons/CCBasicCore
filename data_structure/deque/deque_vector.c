#include "deque_impl.h"
#include "Error.h"
#include "deque_interface.h"
#include <stddef.h>
#include <string.h>
#include "deque_interface.h"
#include "utils_def.h"
#include "vector.h"
#include "deque_vector.h"
#include "result_pack.h"
#include "memory_allocation.h"

static const char* _wrongoverride_vector_message = 
    "Wrong Impl Error happens in CCVector Deque";


static void* _createPtr(size_t size) {
     return CCBasicCoreVector_CreateVector(size);
}

static int _destroyPtr(void* elem){
    CCVector* vector = (CCVector*)elem;
    return CCBasicCoreVector_FreeVector(vector);
}

static void _pushFrontVectorPtr(
    CCDeque* queue, void* elem, size_t size){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
    if(!__CCBasicCoreVector_PushFront(vector, elem, size)){
        _FkOffTheWrongType(queue, "Error in Handling the Size Session");
        return;
    }
}

static void* _topVectorPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
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

static void* _backVectorPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
    size_t size = vector->current_size;
    if(size == 0){
        return NULL; // OK we returns nothing
    }

    ResultPack* elem = CCBasicCoreVector_Get(vector, size - 1);
    if(elem->status_code != CCBasicCore_SUCCESS){
        return NULL;
    }

    return elem->result;
}


static void* _popBackVectorPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    void* elem = _backVectorPtr(queue);
    CCVector* vector = (CCVector*)queue->deque_impl;
    void* result = NULL;
    if(elem){
        if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(vector->pack)){
            result = vector->pack->copier(elem);
        }else{
            result = allocate_memory(vector->element_size);
            memcpy(result, elem, vector->element_size);
        } 
    }
    CCBasicCoreVector_PopBack(vector);
    return result;
}

static void _pushBackVectorPtr(
    CCDeque* queue, void* elem, size_t size){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
    if(!__CCBasicCoreVector_PushBack(vector, elem, size)){
        _FkOffTheWrongType(queue, "Error in Handling the Size Session");
        return;
    }
}


static void* _popFrontVectorPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    void* elem = _topVectorPtr(queue);
    CCVector* vector = (CCVector*)queue->deque_impl;
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


static size_t _sizePtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
    return vector->current_size;
}

static int _emptyPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_VECTOR_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)queue->deque_impl;
    return vector->current_size == 0;
}


static CCDeque_Interface_t vectorQueue_t = {
    _createPtr,
    _pushFrontVectorPtr,
    _popFrontVectorPtr,
    _pushBackVectorPtr,
    _popBackVectorPtr,
    _topVectorPtr,
    _backVectorPtr,
    _sizePtr,
    _emptyPtr,
    _destroyPtr
};

CCDeque* CCBasicCore_CCVectorDequeCreate(size_t size){
    return CCBasicCore_CCDequeCreate(size, &vectorQueue_t, CCDEQUE_VECTOR_IMPL);
}