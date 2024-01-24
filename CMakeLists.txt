cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

set(CMAKE_CXX_STANDART 17)

#set(Boost_NO_SYSTEM_PATHS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_ALL_DYN_LINK OFF)


find_package(Boost COMPONENTS program_options REQUIRED)


include_directories(${Boost_INCLUDE_DIR})
add_executable(bayan main.cpp
			           src/file_list_creator.cpp
					   src/hashers.cpp
					   src/dublicate_finder.cpp
					   src/file.cpp)

add_library(version_lib libs/lib.cpp)

target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bayan PRIVATE version_lib
				${Boost_LIBRARIES})

target_include_directories(bayan PRIVATE include)
#target_link_libraries(bayan ${Boost_LIBRARIES})

target_compile_features(bayan PUBLIC cxx_std_17)


install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
