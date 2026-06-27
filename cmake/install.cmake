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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the compiled library artifact into the platform library directory.
install(
  TARGETS k4c
  EXPORT k4cTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install the public header tree so consumers can include <k4c/...>.
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Generate the version file used by find_package(k4c VERSION ...).
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/k4cConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

# Generate k4cConfig.cmake from the source template.
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/k4cConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/k4cConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/k4c"
)

# Install package config files so find_package(k4c) can locate k4c.
install(
  FILES
    "${PROJECT_BINARY_DIR}/k4cConfig.cmake"
    "${PROJECT_BINARY_DIR}/k4cConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/k4c"
)

# Install the exported target metadata that defines k4c::k4c for consumers.
install(
  EXPORT k4cTargets
  NAMESPACE k4c::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/k4c"
)
