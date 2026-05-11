cmake_minimum_required(VERSION 3.20)

project(algorithms_and_data_structures_c C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

if(NOT DEFINED TARGET)
  message(FATAL_ERROR "Set TARGET relative to src, e.g. -DTARGET=datastruct/array")
endif()

set(TARGET_DIR "${CMAKE_SOURCE_DIR}/src/${TARGET}")

if(NOT EXISTS "${TARGET_DIR}")
  message(FATAL_ERROR "Missing target directory: ${TARGET_DIR}")
endif()

file(GLOB TARGET_SOURCES "${TARGET_DIR}/*.c")

if(NOT TARGET_SOURCES)
  message(FATAL_ERROR "No C source files found in: ${TARGET_DIR}")
endif()

string(REPLACE "/" "-" TARGET_NAME "${TARGET}")

add_executable(${TARGET_NAME} ${TARGET_SOURCES})
