function(cstd_discover_benchmarks)
  file(GLOB BENCH_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "benchmarks/*_bench.c")

  if(NOT BENCH_FILES)
    message(WARNING "No benchmarks found under benchmarks/*_bench.c")
    return()
  endif()

  foreach(BENCH_FILE_REL ${BENCH_FILES})
    get_filename_component(BENCH_BASENAME "${BENCH_FILE_REL}" NAME_WE)
    string(REGEX REPLACE "_bench$" "" BENCH_NAME_RAW "${BENCH_BASENAME}")
    string(REPLACE "_" "-" BENCH_NAME_DASH "${BENCH_NAME_RAW}")

    string(REGEX MATCH "^[^_]+" DATASTRUCT_NAME "${BENCH_NAME_RAW}")
    set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/src/datastruct/${DATASTRUCT_NAME}.c")
    if(NOT EXISTS "${SOURCE_FILE}")
      message(FATAL_ERROR "Missing datastruct source for benchmark ${BENCH_FILE_REL}: ${SOURCE_FILE}")
    endif()

    set(TARGET_NAME "bench-${BENCH_NAME_DASH}")
    add_executable(${TARGET_NAME} "${CMAKE_SOURCE_DIR}/${BENCH_FILE_REL}" ${SOURCE_FILE})
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/benchmarks")
    target_compile_definitions(${TARGET_NAME} PRIVATE _POSIX_C_SOURCE=200809L)

    add_test(NAME "benchmark/${BENCH_NAME_DASH}" COMMAND ${TARGET_NAME})
    set_tests_properties("benchmark/${BENCH_NAME_DASH}" PROPERTIES LABELS "benchmark;datastruct;${DATASTRUCT_NAME}")
  endforeach()
endfunction()
