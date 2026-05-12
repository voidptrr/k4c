cmake_minimum_required(VERSION 3.20)

project(algorithms_and_data_structures_c C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

function(add_dsa_test TARGET_KEY TEST_FILE)
  set(options)
  set(oneValueArgs EXEC_SUFFIX TEST_SUITE TEST_NAME)
  set(multiValueArgs)
  cmake_parse_arguments(ADD_DSA_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  string(REPLACE "/" ";" TARGET_PARTS "${TARGET_KEY}")
  list(LENGTH TARGET_PARTS TARGET_PARTS_LEN)

  if(NOT TARGET_PARTS_LEN EQUAL 2)
    message(FATAL_ERROR "TARGET must be in format <group>/<name>, got: ${TARGET_KEY}")
  endif()

  list(GET TARGET_PARTS 0 TARGET_GROUP)
  list(GET TARGET_PARTS 1 TARGET_NAME_RAW)

  set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/src/${TARGET_NAME_RAW}.c")
  if(NOT EXISTS "${SOURCE_FILE}")
    message(FATAL_ERROR "Missing source file for ${TARGET_KEY}: ${SOURCE_FILE}")
  endif()

  string(REPLACE "/" "-" EXECUTABLE_NAME "${TARGET_KEY}")
  if(DEFINED ADD_DSA_TEST_EXEC_SUFFIX AND NOT ADD_DSA_TEST_EXEC_SUFFIX STREQUAL "")
    set(EXECUTABLE_NAME "${EXECUTABLE_NAME}-${ADD_DSA_TEST_EXEC_SUFFIX}")
  endif()

  add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE} ${TEST_FILE})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")

  set(CTEST_NAME "${EXECUTABLE_NAME}")
  if(DEFINED ADD_DSA_TEST_TEST_NAME AND NOT ADD_DSA_TEST_TEST_NAME STREQUAL "")
    set(CTEST_NAME "${ADD_DSA_TEST_TEST_NAME}")
  endif()
  add_test(NAME ${CTEST_NAME} COMMAND ${EXECUTABLE_NAME})

  set(TEST_LABELS "${TARGET_GROUP}")
  if(DEFINED ADD_DSA_TEST_TEST_SUITE AND NOT ADD_DSA_TEST_TEST_SUITE STREQUAL "")
    list(APPEND TEST_LABELS "${ADD_DSA_TEST_TEST_SUITE}")
  else()
    list(APPEND TEST_LABELS "${TARGET_NAME_RAW}")
  endif()
  set_tests_properties(${CTEST_NAME} PROPERTIES LABELS "${TEST_LABELS}")
endfunction()

if(DEFINED TARGET)
  set(TEST_FILE "${CMAKE_SOURCE_DIR}/tests/${TARGET}_test.c")
  if(NOT EXISTS "${TEST_FILE}")
    file(GLOB TARGET_TEST_FILES "${CMAKE_SOURCE_DIR}/tests/${TARGET}/*_test.c")
    list(LENGTH TARGET_TEST_FILES TARGET_TEST_FILES_LEN)

    if(TARGET_TEST_FILES_LEN EQUAL 1)
      list(GET TARGET_TEST_FILES 0 TEST_FILE)
    elseif(TARGET_TEST_FILES_LEN EQUAL 0)
      message(FATAL_ERROR "Missing test file: ${TEST_FILE} or tests/${TARGET}/*_test.c")
    else()
      message(FATAL_ERROR "Multiple test files found for ${TARGET}; expected exactly one")
    endif()
  endif()
  add_dsa_test("${TARGET}" "${TEST_FILE}")
else()
  file(GLOB TEST_FILES_FLAT RELATIVE "${CMAKE_SOURCE_DIR}" "tests/*/*_test.c")
  file(GLOB TEST_FILES_NESTED RELATIVE "${CMAKE_SOURCE_DIR}" "tests/*/*/*_test.c")
  set(TEST_FILES ${TEST_FILES_FLAT} ${TEST_FILES_NESTED})
  if(NOT TEST_FILES)
    message(WARNING "No tests found under tests/*/*_test.c or tests/*/*/*_test.c")
  endif()

  foreach(TEST_FILE_REL ${TEST_FILES})
    get_filename_component(TEST_DIR "${TEST_FILE_REL}" DIRECTORY)
    string(REPLACE "/" ";" TEST_DIR_PARTS "${TEST_DIR}")
    list(LENGTH TEST_DIR_PARTS TEST_DIR_PARTS_LEN)

    if(TEST_DIR_PARTS_LEN EQUAL 2)
      get_filename_component(TARGET_GROUP "${TEST_DIR}" NAME)
      get_filename_component(TEST_BASENAME "${TEST_FILE_REL}" NAME_WE)
      string(REGEX REPLACE "_test$" "" TARGET_NAME_RAW "${TEST_BASENAME}")
    elseif(TEST_DIR_PARTS_LEN EQUAL 3)
      list(GET TEST_DIR_PARTS 1 TARGET_GROUP)
      list(GET TEST_DIR_PARTS 2 TARGET_NAME_RAW)
      get_filename_component(TEST_BASENAME "${TEST_FILE_REL}" NAME_WE)
      string(REGEX REPLACE "_test$" "" TEST_VARIANT "${TEST_BASENAME}")
    else()
      message(FATAL_ERROR "Unsupported test layout: ${TEST_FILE_REL}")
    endif()

    set(TARGET_KEY "${TARGET_GROUP}/${TARGET_NAME_RAW}")
    if(TEST_DIR_PARTS_LEN EQUAL 3)
      add_dsa_test(
        "${TARGET_KEY}"
        "${CMAKE_SOURCE_DIR}/${TEST_FILE_REL}"
        EXEC_SUFFIX "${TEST_VARIANT}"
        TEST_SUITE "${TARGET_NAME_RAW}"
        TEST_NAME "${TARGET_KEY}/${TEST_VARIANT}"
      )
    else()
      add_dsa_test(
        "${TARGET_KEY}"
        "${CMAKE_SOURCE_DIR}/${TEST_FILE_REL}"
        TEST_NAME "${TARGET_KEY}"
      )
    endif()
  endforeach()
endif()
