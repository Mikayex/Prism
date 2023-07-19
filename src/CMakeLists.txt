add_subdirectory(math)
add_subdirectory(raytracing)

set(APP_FILES
        Application.cpp
        Application.hpp
        main.cpp
        )

add_executable(prism ${APP_FILES})
target_compile_definitions(prism PUBLIC GLFW_INCLUDE_VULKAN GLFW_INCLUDE_NONE)
target_link_libraries(prism glfw glog::glog Vulkan::Headers)
