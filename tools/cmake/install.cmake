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
