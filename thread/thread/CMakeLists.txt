configure_file(
    ${PROJECT_SOURCE_DIR}/platform/thread_impl/common/thread_defs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_defs.h
    COPYONLY
)

set(ThreadSrc thread/thread.c thread/thread.h thread/thread_defs.h PARENT_SCOPE)
