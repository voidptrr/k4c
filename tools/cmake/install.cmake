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
  TARGETS ckit
  EXPORT ckitTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install the public header tree so consumers can include <ckit/...>.
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Generate the version file used by find_package(ckit VERSION ...).
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/ckitConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

# Generate ckitConfig.cmake from the source template.
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/tools/cmake/ckitConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/ckitConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ckit"
)

# Install package config files so find_package(ckit) can locate ckit.
install(
  FILES
    "${PROJECT_BINARY_DIR}/ckitConfig.cmake"
    "${PROJECT_BINARY_DIR}/ckitConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ckit"
)

# Install the exported target metadata that defines ckit::ckit for consumers.
install(
  EXPORT ckitTargets
  NAMESPACE ckit::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ckit"
)
