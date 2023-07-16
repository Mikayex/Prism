add_executable(prism-rt prism-rt.cpp)
target_link_libraries(prism-rt prism glog::glog stb_image_write)