#include "stack_vector.h"
#include "Error.h"
#include "stack_impl.h"
#include "stack_interface.h"
#include "list.h"
#include <stddef.h>

static const char* _wrongoverride_list_message = "Wrong Impl Error happens in CCList Stack";

static void* _createListPtr(size_t elem_size) {
    return CCBasicCore_initCCList(elem_size);
}

static void _pushListPPtr(
    CCStack* stack, void* elem, size_t size){
    if(stack->code != CCSTACK_LIST_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_list_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCList* list = (CCList*)stack->impl;
    
    if(!CCBasicCore_CCListPushBack(list,  elem, size)){
        _FkOffTheWrongType(stack, "Error in Handling the Size Session");
        return;
    }
}

static void* _topListPPtr(CCStack* stack){
    if(stack->code != CCSTACK_LIST_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)stack->impl;

    struct CCListNode* node = CCBasicCore_CCListTail(list);
    if(!node) 
        return NULL;
    return CCBasicCore_GetListNode(node);
}

static void* _popListPPtr(CCStack* stack){
    if(stack->code != CCSTACK_LIST_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_list_message);
        return NULL; // Not a valid code, or things got wrong
    }
    CCList* list = (CCList*)stack->impl;
    return CCBasicCore_CCListPopBack(list);
}

static size_t _sizePtr(CCStack* stack){
    if(stack->code != CCSTACK_LIST_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_list_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)stack->impl;
    return CCBasicCore_CCListSize(list);
}

static int _emptyPtr(CCStack* stack){
    if(stack->code != CCSTACK_LIST_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_list_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCList* list = (CCList*)stack->impl;
    return CCBasicCore_CCListEmpty(list);
}

static int _destroyPtr(void* elem){
    CCList* list = (CCList*)elem;
    return CCBasicCore_destroyCCList(list);
}

static CCStackInterface_t CCListStackInterface  = {
    _createListPtr,
    _pushListPPtr,
    _popListPPtr,
    _topListPPtr,
    _emptyPtr,
    _sizePtr,
    _destroyPtr
};

CCStack* CCBasicCore_CreateListStack(size_t elem_size)
{
    return CCBasicCore_CCStackCreate(
        elem_size, CCSTACK_LIST_IMPL_CODE, 
        &CCListStackInterface);
}