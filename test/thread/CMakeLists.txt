set(TEST_SOURCES
    test_thread.c
    test_mutex.c
    test_condvar.c
)

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE ThreadLibrary)
endforeach()

add_custom_target(run_thread_test
    COMMAND ${CMAKE_COMMAND} -E echo "===== Running all tests ====="
)

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    
    # 每个测试添加到 run_thread_test
    add_custom_command(TARGET run_thread_test
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "===== Running ${TEST_NAME} ====="
        COMMAND $<TARGET_FILE:${TEST_NAME}>
    )
endforeach()
