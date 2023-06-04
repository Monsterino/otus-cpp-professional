cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(2.Filter VERSION ${PROJECT_VESRSION})

configure_file(version.h.in version.h)

add_executable(Filter src/main.cpp
		      src/filter.cpp)
		      



target_include_directories(Filter PRIVATE include
					  libs)

add_library(version_lib libs/lib.cpp)

set_target_properties(Filter PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(Filter PRIVATE version_lib)

if (MSVC)
    target_compile_options(Filter PRIVATE
        /W4
    )
    target_compile_options(version_lib PRIVATE
        /W4
    )
    
else ()
    target_compile_options(Filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(version_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
endif()



install(TARGETS Filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
