set(CMAKE_FOLDER third-parties)

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
