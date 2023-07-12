cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(matrix
    main.cpp
)

target_include_directories(matrix PRIVATE include)

add_library(version_lib libs/lib.cpp)

set_target_properties(matrix PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)



target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(matrix PRIVATE version_lib)

if (MSVC)
    target_compile_options(matrix PRIVATE
        /W4
    )
    target_compile_options(version_lib PRIVATE
        /W4
    )
    
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(version_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
endif()



install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
