cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(editor VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)


add_executable(editor
    main.cpp
    src/controller/controller.cpp
    src/view/view.cpp
    src/model/utils/color.cpp
    src/model/utils/document.cpp
    src/model/shapes/circle.cpp
    src/model/shapes/graphicscene.cpp
    src/model/shapes/graphicsitem.cpp
    src/model/shapes/rectangle.cpp
    src/model/shapes/shape.cpp
)

target_include_directories(editor PRIVATE include
					  libs)
					  
					 
add_library(version_lib libs/lib.cpp)


set_target_properties(editor PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(editor PRIVATE version_lib)

target_compile_features(editor PUBLIC cxx_std_17)



install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
