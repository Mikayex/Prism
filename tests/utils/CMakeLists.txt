add_executable(prism_utils_tests
        Swappable_tests.cpp
        )

target_link_libraries(prism_utils_tests prism_utils gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(prism_utils_tests
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
        )