# template cmake usage, if you want to add file to the 
# building stage, you need to modify here

set(MemoryAllocateSrc ${MemoryAllocateSrc} 
    ${CMAKE_CURRENT_LIST_DIR}/memory_allocation_impl.c
    PARENT_SCOPE)
