#include "deque_interface.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"


CCDeque* CCBasicCore_CCDequeCreate(
    size_t size, CCDeque_Interface_t* interface, CCDequeImplCode_t code)
{
    CCDeque* queue = allocate_one(CCDeque);
    queue->code = code;
    queue->interface = interface;
    queue->deque_impl = queue->interface->create_ptr(size);
    return queue;
}

int CCBasicCore_CCDequeDestroy(CCDeque* queue){
    int result_impl = queue->interface->destroy_ptr(queue->deque_impl);
    int result = free_memory(queue);
    return result && result_impl;
}
