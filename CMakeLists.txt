cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulkmt VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

set(SOURCES 
	src/writer.cpp
	src/context.cpp
	src/logger.cpp
	src/accumulator.cpp
	src/parser.cpp
)





add_executable(bulkmt main.cpp)
add_library(async SHARED lib/async.cpp ${SOURCES} )
add_library(version_lib lib/lib.cpp)

set_target_properties(bulkmt async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulkmt PRIVATE lib include)
target_include_directories(async PRIVATE include)
target_include_directories(version_lib PRIVATE "${CMAKE_BINARY_DIR}")

					 
target_link_libraries(bulkmt PRIVATE async)
target_link_libraries(bulkmt PRIVATE version_lib)

target_compile_features(bulkmt PUBLIC cxx_std_17)


install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
