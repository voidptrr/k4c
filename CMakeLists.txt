cmake_minimum_required(VERSION 3.20)

project(ckit VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB_RECURSE CKIT_SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(ckit ${CKIT_SRC_FILES})
add_library(ckit::ckit ALIAS ckit)

target_compile_features(ckit PUBLIC c_std_17)
target_include_directories(
  ckit
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(
  ckit
  PROPERTIES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME ckit
)

include(tools/cmake/install.cmake)

if(BUILD_TESTING)
  include(tools/cmake/tests.cmake)
  ck_discover_tests()
endif()
