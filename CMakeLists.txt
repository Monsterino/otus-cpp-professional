cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(mapreduce VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)


add_executable(mapreduce main.cpp
        src/splitter.cpp
        src/mapper.cpp
        src/functions.cpp
        src/reader.cpp
        src/shuffler.cpp
        src/reducer.cpp)

add_library(version_lib libs/lib.cpp)

set_target_properties(mapreduce PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


target_include_directories(mapreduce PRIVATE include)

target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(mapreduce PRIVATE version_lib)

install(TARGETS mapreduce RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)