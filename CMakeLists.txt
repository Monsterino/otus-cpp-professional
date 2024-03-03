cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

set(CMAKE_CXX_STANDART 17)

#set(Boost_NO_SYSTEM_PATHS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_ALL_DYN_LINK OFF)


if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif (MSVC)


set(SOURCES 
	src/writer.cpp
	src/context.cpp
	src/logger.cpp
	src/accumulator.cpp
	src/parser.cpp
	src/pusher.cpp
)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(bulk_server main.cpp
	                       network/src/server.cpp
						   network/src/session.cpp)

add_library(async SHARED lib/async.cpp ${SOURCES} )
add_library(version_lib lib/lib.cpp)

set_target_properties(bulk_server async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async PRIVATE include)
target_include_directories(version_lib PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(bulk_server PRIVATE lib include network/include)


target_link_libraries(bulk_server PRIVATE async)
target_link_libraries(bulk_server PRIVATE ${Boost_LIBRARIES})
target_link_libraries(bulk_server PRIVATE version_lib)
target_link_libraries(async PRIVATE ${Boost_LIBRARIES})	 


install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)