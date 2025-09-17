set(TEST_SOURCES
    test_vector.c
    test_vector_deep.c
    test_list_with_deep.c
    test_vector_stack.c
    test_list_stack.c
    test_queue.c
    test_deque_list.c
    test_deque_vector.c
    test_map.c
    test_unorder_map.c
    test_cstring.c
    test_set.c
    test_unordered_set.c
)

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE DataStructure)
endforeach()

add_custom_target(run_all_tests
    COMMAND ${CMAKE_COMMAND} -E echo "===== Running all tests ====="
)

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    
    # 每个测试添加到 run_all_tests
    add_custom_command(TARGET run_all_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "===== Running ${TEST_NAME} ====="
        COMMAND $<TARGET_FILE:${TEST_NAME}>
    )
endforeach()
