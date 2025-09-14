#include "stack_interface.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
CCStack* CCBasicCore_CCStackCreate(
    size_t elem_size, CCStackImplCode_t impl_code, CCStackInterface_t* interfaces)
{
    CCStack* stack = allocate_one(CCStack);
    stack->code = impl_code;
    stack->interfaces = interfaces;
    stack->impl = interfaces->creator_ptr(elem_size);
    return stack;
}

int CCBasicCore_CCStackDestroy(CCStack* stack){
    int result_impl = stack->interfaces->destroy_op(stack->impl);
    int result_stack = free_memory(stack);
    return result_impl && result_stack;
}
