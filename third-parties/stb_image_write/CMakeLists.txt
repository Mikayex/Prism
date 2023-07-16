add_library(stb_image_write STATIC stb_image_write.h stb_image_write.cpp)
target_include_directories(stb_image_write PUBLIC .)
target_link_libraries(stb_image_write PUBLIC glog::glog)

if (WIN32)
    target_compile_definitions(stb_image_write PUBLIC STBIW_WINDOWS_UTF8)
endif ()