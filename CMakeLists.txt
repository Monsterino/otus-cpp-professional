cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulkmt VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif (MSVC)


find_package(GTest REQUIRED)


set(SOURCES 
	src/writer.cpp
	src/context.cpp
	src/logger.cpp
	src/accumulator.cpp
	src/parser.cpp
)


add_executable(bulkmt main.cpp)

add_library(async SHARED lib/async.cpp
						 ${SOURCES} )

add_library(version_lib lib/lib.cpp)


add_executable(tests
			   tests/cpp/test.cpp
			   tests/cpp/test_logger.cpp
			   tests/cpp/test_writer.cpp
			   tests/cpp/test_accumulator.cpp
			   tests/cpp/test_parser.cpp
			   tests/cpp/utils/test_classes.cpp
			   tests/cpp/utils/barrier.cpp
			   tests/cpp/utils/utils.cpp)



set_target_properties(bulkmt async tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulkmt PRIVATE lib include)
target_include_directories(async PRIVATE include)
target_include_directories(tests PRIVATE lib 
										 include 
										 tests/include
										 ${GTEST_INCLUDE_DIRS})
target_include_directories(version_lib PRIVATE "${CMAKE_BINARY_DIR}")


					 
target_link_libraries(bulkmt PRIVATE async)
target_link_libraries(bulkmt PRIVATE version_lib)
target_link_libraries(tests PRIVATE  ${GTEST_LIBRARIES} )
target_link_libraries(tests PRIVATE  async )


target_compile_features(bulkmt PUBLIC cxx_std_17)

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(tests tests)

