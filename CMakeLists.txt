cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VESRION})

find_package(GTest)

configure_file(version.h.in version.h)


add_executable(helloworld src/main.cpp)
target_include_directories(helloworld PRIVATE libs)


add_library(version_lib libs/lib.cpp)
add_executable(test_version test/test_version.cpp
			    test/tests.cpp)

set_target_properties(helloworld version_lib test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_include_directories(
    test_version PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
)

target_include_directories(
    test_version PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
)

target_include_directories(
    test_version PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
    test_version PRIVATE
    ${GTEST_BOTH_LIBRARIES}
)



target_link_libraries(helloworld PRIVATE version_lib)
target_link_libraries(test_version  PRIVATE version_lib)


if (MSVC)
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(version_lib PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(version_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS helloworld RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
