add_subdirectory(math)
add_subdirectory(raytracing)

add_executable(prism-rt prism-rt.cpp)
target_link_libraries(prism-rt raytracing glog::glog stb_image_write)
