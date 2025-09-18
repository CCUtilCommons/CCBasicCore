set(UtilsSrc
    errors/Error.h errors/Error.c
    memory_behave/memory_behave.c 
    memory_behave/memory_behave.h
    result_pack.c result_pack.h 
    utils_def.c utils_def.h 
    compares.h)

add_library(UtilsLibrary STATIC ${UtilsSrc})
target_include_directories(UtilsLibrary PUBLIC . ./errors)
target_link_libraries(UtilsLibrary PRIVATE PlatformCore)