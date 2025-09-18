
# collect the THREADLIB_SOURCES and dump the platform Interface Library
message(STATUS "Now Scan src according to the OS_TYPE as ${OS_TYPE}")
if(OS_TYPE STREQUAL "OS_WINDOWS")
    message(STATUS "OS_TYPE: ${OS_TYPE}, this will make u run into the windows folder")
    add_subdirectory(windows)
elseif(OS_TYPE STREQUAL "OS_LINUX" OR OS_TYPE STREQUAL "OS_MACOS")
    message(STATUS "OS_TYPE: ${OS_TYPE}, this will make u run into the posix folder")
    add_subdirectory(posix)
else()
    message(STATUS "OS_TYPE: ${OS_TYPE}, this will make u run into the other folder to scan the implements")
    add_subdirectory(other)
endif()

message(STATUS "The ThreadIMPL Library is configuring OK!")