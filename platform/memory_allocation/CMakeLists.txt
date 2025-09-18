# >>> List Methods begin
set(SUBDIR_native "native")

# Add your defines here
# Format is SUBDIR_${FOLDER_NAME} "${FOLDER_NAME}"
# Example: if you add memory_pool folder here, you should write 
# set(SUBDIR_memory_pool "memory_pool")
# Take it easy!

# <<< List Methods end

set(SUBDIR_VAR "SUBDIR_${MemoryAllocateMethod}")

if(DEFINED ${SUBDIR_VAR})
    message(STATUS "Adding subdirectory for selecting memory allocator: ${MemoryAllocateMethod}")
    add_subdirectory(${MemoryAllocateMethod})
    set(MemoryAllocateSrc ${MemoryAllocateSrc} memory_allocation/memory_allocation.h PARENT_SCOPE)
    message(STATUS "For Dump usage, memory allocators includes ${MemoryAllocateSrc}")
    add_library(PlatformCore STATIC ${MemoryAllocateSrc} ${ThreadSrc})
    target_include_directories(PlatformCore PUBLIC .)
else()
    message(FATAL_ERROR "Unknown feature: ${MemoryAllocateMethod}, view the ")
endif()




