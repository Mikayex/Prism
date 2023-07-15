set(PRISM_MATH
        math/Point2.cpp
        math/Point2.hpp
        math/Point3.cpp
        math/Point3.hpp
        math/Vector2.cpp
        math/Vector2.hpp
        math/Vector3.cpp
        math/Vector3.hpp
        )
source_group(Math FILES ${PRISM_MATH})

add_library(prism STATIC ${PRISM_MATH})
target_include_directories(prism PUBLIC .)
target_link_libraries(prism PUBLIC glog::glog)

add_subdirectory(executables)
