message(STATUS "Configuring the Memory Allocations Sessions")

add_subdirectory(memory_allocation)
set(${MemoryAllocateSrc} ${MemoryAllocateSrc})
message("MemoryAllocateSrc src: ${MemoryAllocateSrc}")

add_library(PlatformCore STATIC ${MemoryAllocateSrc})
target_include_directories(PlatformCore PUBLIC memory_allocation)

