cmake_minimum_required(VERSION 3.10)

# C Settings for the Memory Settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")
# Projects
project(CCBasicCore VERSION 0.1.0 LANGUAGES C CXX)

message(STATUS "Ready to configure the platform relative sessions")

# >>> --- Ready to configure the platform settings
set(MemoryAllocateMethod "native" CACHE STRING "Memory Allocate Behavior")

message(STATUS "Memory Allocate method is ${MemoryAllocateMethod} method")
add_subdirectory(platform)


# >>> --- Ready to configure the data structure library
message(STATUS "Configuring Data Structure library")
add_subdirectory(data_structure)

message(STATUS "Test shell be built")
add_subdirectory(test)