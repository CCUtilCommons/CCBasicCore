
set(THREADLIB_SOURCES 
    posix_thread_bkend.c
    ../common/thread_impl_interface.h)

message(STATUS "Dump Thread IMPL Backend: ${THREADLIB_SOURCES}")

add_library(ThreadIMPL STATIC ${THREADLIB_SOURCES})

# Link the Memory And Platform Simple OP Library
target_link_libraries(ThreadIMPL PUBLIC PlatformCore UtilsLibrary)
target_include_directories(ThreadIMPL PUBLIC ../posix ../common)

find_package(Threads REQUIRED)
if(Threads_FOUND)
    target_link_libraries(ThreadIMPL PRIVATE Threads::Threads)
endif()