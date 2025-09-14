set(DataStructure_Src "")
add_subdirectory(utils)
add_subdirectory(vector)
add_subdirectory(errors)
add_subdirectory(list)
add_subdirectory(stack)
message("data structure_src: ${DataStructure_Src}")

add_library(DataStructure STATIC ${DataStructure_Src})
target_include_directories(DataStructure PUBLIC 
    vector utils errors list stack)
target_include_directories(DataStructure PRIVATE ../platform)
target_link_libraries(DataStructure PRIVATE PlatformCore)