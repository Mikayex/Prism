set(UTILS
        Swappable.cpp
        Swappable.hpp
        )

add_library(prism_utils STATIC ${UTILS})
target_include_directories(prism_utils PUBLIC ..)
