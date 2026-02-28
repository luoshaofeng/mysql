# Install script for directory: /tmp/mysql/extra/abseil/abseil-cpp-20230802.1/absl

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local/mysql")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/base/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/algorithm/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/cleanup/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/container/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/crc/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/debugging/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/flags/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/functional/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/hash/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/log/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/memory/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/meta/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/numeric/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/profiling/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/random/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/status/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/strings/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/synchronization/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/time/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/types/cmake_install.cmake")
  include("/tmp/mysql/cmake-build-mysql-docker/extra/abseil/abseil-cpp-20230802.1/absl/utility/cmake_install.cmake")

endif()

