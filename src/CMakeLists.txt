set(PRISM_FILES
        Ray.cpp
        Ray.hpp
        )

set(PRISM_MATH
        math/Float.hpp
        math/Vector2.cpp
        math/Vector2.hpp
        math/Vector3.cpp
        math/Vector3.hpp
        math/Functions.hpp)
source_group(Math FILES ${PRISM_MATH})

set(PRISM_SHAPES
        shapes/Shape.cpp
        shapes/Shape.hpp
        shapes/Sphere.cpp
        shapes/Sphere.hpp
        )
source_group(Shapes FILES ${PRISM_SHAPES})

add_library(prism STATIC ${PRISM_FILES} ${PRISM_MATH} ${PRISM_SHAPES})
target_include_directories(prism PUBLIC .)
target_link_libraries(prism PUBLIC glog::glog)

add_subdirectory(executables)
