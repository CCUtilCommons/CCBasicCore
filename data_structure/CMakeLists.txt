set(DataStructure_Src "")
add_subdirectory(utils)
add_subdirectory(vector)
add_subdirectory(errors)
add_subdirectory(list)
add_subdirectory(stack)
add_subdirectory(queue)
add_subdirectory(deque)
add_subdirectory(set)
option(USE_TREAP_ACC "Use Treap Implementation for MapNode" ON)
add_subdirectory(map)

option(USE_STRING_SSO "Use the String Small Object To Accelerate Process" ON)
add_subdirectory(string)
# message("data structure_src: ${DataStructure_Src}")

add_library(DataStructure STATIC ${DataStructure_Src})
target_include_directories(DataStructure PUBLIC 
    vector utils errors list stack queue deque
    map string set)
target_include_directories(DataStructure PRIVATE ../platform)
target_link_libraries(DataStructure PRIVATE PlatformCore)
target_compile_definitions(DataStructure PRIVATE $<$<BOOL:${USE_TREAP_ACC}>:USE_TREAP_ACC>)
target_compile_definitions(DataStructure PRIVATE $<$<BOOL:${USE_STRING_SSO}>:USE_STRING_SSO>)