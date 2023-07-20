add_subdirectory(math)
add_subdirectory(raytracing)

set(APP_FILES
        Application.cpp
        Application.hpp
        VulkanDevice.cpp
        VulkanDevice.hpp
        VulkanUtils.cpp
        VulkanUtils.hpp
        main.cpp
        )

add_executable(prism ${APP_FILES})
target_compile_definitions(prism PUBLIC
        GLFW_INCLUDE_VULKAN
        GLFW_INCLUDE_NONE
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        )

target_link_libraries(prism glfw glog::glog Vulkan::Headers)
