cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(bulk VERSION ${PROJECT_VESRION})


configure_file(version.h.in version.h)

add_executable(bulk
    main.cpp 
    src/bulk_command.cpp
    src/execute_handler.cpp
    src/logger_handler.cpp
    src/main_handler.cpp
    src/command_executor.cpp
)

add_library(version_lib libs/lib.cpp)

set_target_properties(bulk PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)


target_include_directories(bulk PRIVATE include)

target_link_libraries(bulk PRIVATE version_lib)


target_compile_features(bulk PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(version_lib PRIVATE
        /W4
    )
    
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(version_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
endif()



install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
