enable_testing()

add_executable(tests
    test_my_vector.cpp
    test_my_array.cpp
    test_my_smart_ptrs.cpp
)
target_link_libraries(tests PRIVATE my_containers gtest_main)
add_test(NAME my_container_tests COMMAND tests)
