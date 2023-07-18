set(MATH
        Float.hpp
        Vector2.cpp
        Vector2.hpp
        Vector3.cpp
        Vector3.hpp
        Functions.hpp)

add_library(math STATIC ${MATH})
target_include_directories(math PUBLIC ..)
target_link_libraries(math PUBLIC glog::glog)
