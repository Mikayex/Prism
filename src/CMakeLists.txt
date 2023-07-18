add_subdirectory(math)
add_subdirectory(raytracing)

set(APP_FILES
        Application.cpp
        Application.hpp
        main.cpp
        )

add_executable(prism ${APP_FILES})
target_link_libraries(prism glfw glog::glog)
