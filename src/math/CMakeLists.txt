set(PRISM_MATH_HPP
        Vector2.hpp
        Vector3.hpp
        )

set(PRISM_MATH_CPP
        Vector2.cpp
        Vector3.cpp
        )

add_library(prism_math STATIC ${PRISM_MATH_HPP} ${PRISM_MATH_CPP})
target_include_directories(prism_math PUBLIC ..)
target_link_libraries(prism_math PUBLIC glog::glog)