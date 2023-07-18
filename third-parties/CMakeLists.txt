set(CMAKE_FOLDER third-parties)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# Google Log
set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
set(WITH_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
if (WIN32)
    #add_compile_definitions(GOOGLE_GLOG_DLL_DECL=)
endif ()
add_subdirectory(glog)

# STB Image Write
add_subdirectory(stb_image_write)

# Google Test
if (PRISM_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    add_subdirectory(gtest)
    mark_as_advanced(
            BUILD_GTEST BUILD_SHARED_LIBS
            gtest_build_samples gtest_build_tests
            gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )
endif ()

# Vulkan Headers
add_subdirectory(vulkan-headers)
