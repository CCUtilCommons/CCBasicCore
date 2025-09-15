#include "queue_interface.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"


CCQueue* CCBasicCore_CCQueueCreate(
    size_t size, CCQueue_Interface_t* interface, CCQueueImplCode_t code)
{
    CCQueue* queue = allocate_one(CCQueue);
    queue->code = code;
    queue->interface = interface;
    queue->queue_impl = queue->interface->create_ptr(size);
    return queue;
}

int CCBasicCore_CCQueueDestroy(CCQueue* queue){
    int result_impl = queue->interface->destroy_ptr(queue->queue_impl);
    int result = free_memory(queue);
    return result && result_impl;
}
