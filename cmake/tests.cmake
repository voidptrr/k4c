# MIT License
#
# Copyright (c) 2026 Tommaso Bruno
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.c"
)
list(SORT TEST_SOURCES)

add_custom_target(k4c_ctests)

foreach(test_source IN LISTS TEST_SOURCES)
    file(RELATIVE_PATH test_relative "${CMAKE_CURRENT_SOURCE_DIR}/tests" "${test_source}")
    string(REGEX REPLACE "_test\\.c$" "" test_name "${test_relative}")
    string(REPLACE "/" "-" test_name "${test_name}")
    string(REPLACE "_" "-" test_name "${test_name}")

    add_executable("${test_name}" "${test_source}")
    target_include_directories("${test_name}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    )
    target_compile_options("${test_name}" PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
    target_link_libraries("${test_name}" PRIVATE k4c::k4c)
    add_dependencies(k4c_ctests "${test_name}")
    add_test(NAME "${test_name}" COMMAND "${test_name}")
endforeach()
