cmake_minimum_required(VERSION 3.10)

# C Settings for the Memory Settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")
# Projects
project(CCBasicCore VERSION 0.1.0 LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(platform_detection)

set_os_definition()

message(STATUS "Ready to configure the platform relative sessions")

# >>> --- Ready to configure the platform settings
set(MemoryAllocateMethod "native" CACHE STRING "Memory Allocate Behavior")

message(STATUS "Memory Allocate method is ${MemoryAllocateMethod} method")
add_subdirectory(platform)

# >>> --- Ready to configure utils folder library
message(STATUS "Configuring Utils library")
add_subdirectory(utils)

# >>> --- Ready to configure the data structure library
message(STATUS "Configuring Data Structure library")
add_subdirectory(data_structure)

# >>> Ready to configure the thread library
add_subdirectory(thread)

# >>> Ready to configure examples
add_subdirectory(example)

message(STATUS "Test shell be built")
add_subdirectory(test)