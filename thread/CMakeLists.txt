message(STATUS "Configuring the thread dependencies")

# Collect ThreadSrc And ready for builting the library for the thread
add_subdirectory(thread)
set(${ThreadSrc} ${ThreadSrc})
message("ThreadSrc: ${ThreadSrc}")
# Collect MutexSrc and Ready for  builting the library for the mutex
add_subdirectory(mutex)
set(${MutexSrc} ${MutexSrc})
message("MutexSrc: ${MutexSrc}")
# Collect CVSrc and Ready for  builting the library for the cv
add_subdirectory(condition_variable)
set(${CVSrc} ${CVSrc})
message("CVSrc: ${CVSrc}")



add_library(ThreadLibrary ${ThreadSrc} ${MutexSrc} ${CVSrc})
target_link_libraries(ThreadLibrary PRIVATE ThreadIMPL)
target_include_directories( ThreadLibrary PUBLIC 
                            mutex thread condition_variable)