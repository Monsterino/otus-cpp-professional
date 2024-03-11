cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif (MSVC)


find_package(GTest REQUIRED)

find_package(Boost COMPONENTS system REQUIRED)


set(SOURCES 
	src/database.cpp
	src/command_parser.cpp
	src/error_codes.cpp
	src/table.cpp
	src/utils/utils.cpp
)

add_executable(join_server main.cpp
	                       network/src/server.cpp
						   network/src/session.cpp
						   ${SOURCES})

add_executable(u_tests
			   tests/cpp/test.cpp
			   tests/cpp/test_messages.cpp
			   tests/cpp/test_table.cpp
			   tests/cpp/test_database.cpp
			   tests/cpp/utils/barrier.cpp
			    ${SOURCES})


add_executable(client main_client.cpp)


set_target_properties(join_server u_tests client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

include_directories(${Boost_INCLUDE_DIR})

target_include_directories(join_server PRIVATE 
									   include 
									   network/include
									   include/utils)

target_include_directories(u_tests PRIVATE  
								   include 
								   tests/include
								   tests/gtest/include
								   ${GTEST_INCLUDE_DIRS})

target_include_directories(client PRIVATE 
								  network/include)

target_link_libraries(u_tests  PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(join_server PRIVATE ${Boost_LIBRARIES})
target_link_libraries(client PRIVATE ${Boost_LIBRARIES}) 

target_compile_features(join_server PUBLIC cxx_std_17)

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(u_tests u_tests)

