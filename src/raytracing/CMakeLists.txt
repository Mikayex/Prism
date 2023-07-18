set(RT_FILES
        Ray.cpp
        Ray.hpp
        )

set(RT_SHAPES
        shapes/Shape.cpp
        shapes/Shape.hpp
        shapes/Sphere.cpp
        shapes/Sphere.hpp
        )
source_group(Shapes FILES ${RT_SHAPES})

add_library(raytracing STATIC ${RT_FILES} ${RT_SHAPES})
target_include_directories(raytracing PUBLIC .)
target_link_libraries(raytracing PUBLIC math glog::glog)
