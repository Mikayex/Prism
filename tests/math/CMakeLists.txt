add_executable(
        prism_math_tests
        Vector2_tests.cpp
        Vector3_tests.cpp
)

target_link_libraries(prism_math_tests prism_math gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(prism_math_tests
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
        )