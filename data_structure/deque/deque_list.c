#include "deque_impl.h"
#include "Error.h"
#include "deque_interface.h"
#include <stddef.h>
#include <string.h>
#include "deque_interface.h"
#include "utils_def.h"
#include "list.h"
#include "deque_list.h"
#include "result_pack.h"
#include "memory_allocation.h"

static const char* _wrongoverride_list_message = 
    "Wrong Impl Error happens in CCList Deque";


static void* _createPtr(size_t size) {
     return CCBasicCore_initCCList(size);
}

static int _destroyPtr(void* elem){
    CCList* list = (CCList*)elem;
    return CCBasicCore_destroyCCList(list);
}

static void _pushFrontListPtr(
    CCDeque* queue, void* elem, size_t size){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    if(!CCBasicCore_CCListPushFront(list, elem, size)){
        _FkOffTheWrongType(queue, "Error in Handling the Size Session");
        return;
    }
}

static void* _topListPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    size_t size = list->size;
    if(size == 0){
        return NULL; // OK we returns nothing
    }

    return CCBasicCore_GetListNode(list->head);
}

static void* _backListPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    size_t size = list->size;
    if(size == 0){
        return NULL; // OK we returns nothing
    }

    return CCBasicCore_GetListNode(list->tail);
}

static void* _popBackListPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    void* elem = _backListPtr(queue);
    CCList* list = (CCList*)queue->deque_impl;
    void* result = NULL;
    if(elem){
        if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(list->pack)){
            result = list->pack->copier(elem);
        }else{
            result = allocate_memory(list->element_size);
            memcpy(result, elem, list->element_size);
        } 
    }
    CCBasicCore_CCListPopBack(list);
    return result;
}

static void _pushBackListPtr(
    CCDeque* queue, void* elem, size_t size){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    if(!CCBasicCore_CCListPushBack(list, elem, size)){
        _FkOffTheWrongType(queue, "Error in Handling the Size Session");
        return;
    }
}



static void* _popFrontListPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    void* elem = _topListPtr(queue);
    CCList* list = (CCList*)queue->deque_impl;
    void* result = NULL;
    if(elem){
        if(CCBasicCoreCCSpecialDefinedPack_OwnsSpecialCopy(list->pack)){
            result = list->pack->copier(elem);
        }else{
            result = allocate_memory(list->element_size);
            memcpy(result, elem, list->element_size);
        } 
    }
    CCBasicCore_CCListPopFront(list);
    return result;
}


static size_t _sizePtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    return list->size;
}

static int _emptyPtr(CCDeque* queue){
    if(queue->code != CCDEQUE_LIST_IMPL){
        _FkOffTheWrongOverrider(queue, 
            _wrongoverride_list_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)queue->deque_impl;
    return list->size == 0;
}


static CCDeque_Interface_t listQueue_t = {
    _createPtr,
    _pushFrontListPtr,
    _popFrontListPtr,
    _pushBackListPtr,
    _popBackListPtr,
    _topListPtr,
    _backListPtr,
    _sizePtr,
    _emptyPtr,
    _destroyPtr
};

CCDeque* CCBasicCore_CCListDequeCreate(size_t size){
    return CCBasicCore_CCDequeCreate(size, &listQueue_t, CCDEQUE_LIST_IMPL);
}