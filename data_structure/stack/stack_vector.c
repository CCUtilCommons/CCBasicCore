#include "stack_vector.h"
#include "Error.h"
#include "result_pack.h"
#include "stack_impl.h"
#include "stack_interface.h"
#include "vector.h"
#include <stddef.h>

static const char* _wrongoverride_vector_message = 
    "Wrong Impl Error happens in CCVector Stack";


static void* _createVectorPtr(size_t elem_size) {
    return CCBasicCoreVector_CreateVector(elem_size);
}

static void _pushVectorPtr(
    CCStack* stack, void* elem, size_t size){
    if(stack->code != CCSTACK_VECTOR_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_vector_message);
        return; // Not a valid code, or things got wrong
    }

    // Pass the Impl Check
    CCVector* vector = (CCVector*)stack->impl;
    if(!__CCBasicCoreVector_PushBack(vector, elem, size)){
        _FkOffTheWrongType(stack, "Error in Handling the Size Session");
        return;
    }
}

static void* _topVectorPtr(CCStack* stack){
    if(stack->code != CCSTACK_VECTOR_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_vector_message);
        return NULL; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)stack->impl;
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

static void* _popVectorPtr(CCStack* stack){
    void* elem = _topVectorPtr(stack);
    CCVector* vector = (CCVector*)stack->impl;
    CCBasicCoreVector_PopBack(vector);
    return elem;
}

static size_t _sizePtr(CCStack* stack){
    if(stack->code != CCSTACK_VECTOR_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)stack->impl;
    return vector->current_size;
}

static int _emptyPtr(CCStack* stack){
    if(stack->code != CCSTACK_VECTOR_IMPL_CODE){
        _FkOffTheWrongOverrider(stack, 
            _wrongoverride_vector_message);
        return 0; // Not a valid code, or things got wrong
    }
    // Pass the Impl Check
    CCVector* vector = (CCVector*)stack->impl;
    return vector->current_size == 0;
}

static int _destroyPtr(void* elem){
    CCVector* vector = (CCVector*)elem;
    return CCBasicCoreVector_FreeVector(vector);
}

static CCStackInterface_t CCVectorStackInterface  = {
    _createVectorPtr,
    _pushVectorPtr,
    _popVectorPtr,
    _topVectorPtr,
    _emptyPtr,
    _sizePtr,
    _destroyPtr
};

CCStack* CCBasicCore_CreateVectorStack(size_t elem_size)
{
    return CCBasicCore_CCStackCreate(
        elem_size, CCSTACK_VECTOR_IMPL_CODE, 
        &CCVectorStackInterface);
}