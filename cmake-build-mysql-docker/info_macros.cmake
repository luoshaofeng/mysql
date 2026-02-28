# Copyright (c) 2011, 2025, Oracle and/or its affiliates.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2.0,
# as published by the Free Software Foundation.
#
# This program is designed to work with certain software (including
# but not limited to OpenSSL) that is licensed under separate terms,
# as designated in a particular file or component or in included license
# documentation.  The authors of MySQL hereby grant you an additional
# permission to link the program and your derivative works with the
# separately licensed software that they have either included with
# the program or referenced in the documentation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License, version 2.0, for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA 


# Handle/create the "INFO_*" files describing a MySQL (server) binary.
# This is part of the fix for bug#42969.


# Several of cmake's variables need to be translated from '@' notation
# to '${}', this is done by the "configure" call in top level "CMakeLists.txt".
# If further variables are used in this file, add them to this list.

SET(VERSION "8.0.45")
SET(MAJOR_VERSION "8")
SET(MINOR_VERSION "0")
SET(PATCH_VERSION "45")
SET(EXTRA_VERSION "")
SET(CMAKE_SOURCE_DIR "/tmp/mysql")
SET(CMAKE_BINARY_DIR "/tmp/mysql/cmake-build-mysql-docker")
SET(CMAKE_GENERATOR "Unix Makefiles")
SET(GIT_EXECUTABLE "/usr/bin/git")
SET(CMAKE_HOST_SYSTEM "Linux-6.10.14-linuxkit")
SET(CMAKE_HOST_SYSTEM_PROCESSOR "aarch64")
SET(CWD_DEFINITIONS "_GNU_SOURCE;_FILE_OFFSET_BITS=64;BOOST_NO_CXX98_FUNCTION_BASE;__STDC_LIMIT_MACROS;__STDC_FORMAT_MACROS;_USE_MATH_DEFINES;LZ4_DISABLE_DEPRECATE_WARNINGS;HAVE_TLSv13")

SET(CMAKE_C_FLAGS "-fno-omit-frame-pointer -ffp-contract=off  -moutline-atomics -Wall -Wextra -Wformat-security -Wvla -Wundef -Wmissing-format-attribute -Wwrite-strings -Wjump-misses-init -Wstringop-truncation -Wmissing-include-dirs -Werror")
SET(CMAKE_CXX_FLAGS "-std=c++17 -fno-omit-frame-pointer -ffp-contract=off  -moutline-atomics -march=armv8-a+crc -Wall -Wextra -Wformat-security -Wvla -Wundef -Wmissing-format-attribute -Woverloaded-virtual -Wcast-qual -Wimplicit-fallthrough=5 -Wstringop-truncation -Wsuggest-override -Wmissing-include-dirs -Wextra-semi -Wlogical-op -Werror")

SET(CMAKE_C_FLAGS_DEBUG "-DSAFE_MUTEX -DENABLED_DEBUG_SYNC -g")
SET(CMAKE_CXX_FLAGS_DEBUG "-DSAFE_MUTEX -DENABLED_DEBUG_SYNC -g")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-ffunction-sections -fdata-sections -O2 -g -DNDEBUG -g1")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-ffunction-sections -fdata-sections -O2 -g -DNDEBUG -g1")
SET(CMAKE_C_FLAGS_RELEASE "-ffunction-sections -fdata-sections -O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-ffunction-sections -fdata-sections -O3 -DNDEBUG")
SET(CMAKE_C_FLAGS_MINSIZEREL "-ffunction-sections -fdata-sections -Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-ffunction-sections -fdata-sections -Os -DNDEBUG")

SET(CMAKE_CXX_COMPILER_ID "GNU")
SET(CMAKE_CXX_COMPILER_VERSION "11.4.0")
SET(HAVE_BUILD_ID_SUPPORT "1")
SET(WITHOUT_SERVER "OFF")

SET(COMPILER_ID_AND_VERSION
  "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

MACRO(STRING_APPEND STRING_VAR INPUT)
  SET(${STRING_VAR} "${${STRING_VAR}}${INPUT}")
ENDMACRO()

# Create an "INFO_SRC" file with information about the source (only).
# We use "git log", if possible, and the "VERSION" contents.
#
# Outside development (git tree), the "INFO_SRC" file will not be modified
# provided it exists (from "make dist" or a source tarball creation).

MACRO(CREATE_INFO_SRC target_dir)
  SET(INFO_SRC "${target_dir}/INFO_SRC")

  IF(GIT_EXECUTABLE AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
    # Sources are in a GIT repository: Always update.
    EXECUTE_PROCESS(
      COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE bname
      )

    STRING(TIMESTAMP bdate "%Y-%m-%d %H:%M:%SZ" UTC)

    SET(GIT_PRETTY_ARG "")
    STRING_APPEND(GIT_PRETTY_ARG "commit: %H%ndate: %ci%n")
    STRING_APPEND(GIT_PRETTY_ARG "created at: ${bdate}%n")
    STRING_APPEND(GIT_PRETTY_ARG "branch: ${bname}")

    EXECUTE_PROCESS(
      COMMAND ${GIT_EXECUTABLE} log -1
      --pretty="${GIT_PRETTY_ARG}"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE VERSION_INFO
    )

    ## Output from git is quoted with "", remove them.
    STRING(REPLACE "\"" "" VERSION_INFO "${VERSION_INFO}")
    FILE(WRITE ${INFO_SRC} "${VERSION_INFO}\n")
    # to debug, add: FILE(APPEND ${INFO_SRC} "\nResult ${RESULT}\n")
    # For better readability ...
    FILE(APPEND ${INFO_SRC}
      "MySQL source ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
    IF(DEFINED EXTRA_VERSION)
      FILE(APPEND ${INFO_SRC} "${EXTRA_VERSION}")
    ENDIF()
    FILE(APPEND ${INFO_SRC} "\n")

  ELSEIF(EXISTS ${INFO_SRC})
    # Outside a git tree, there is no need to change an existing "INFO_SRC",
    # it cannot be improved.
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/Docs/INFO_SRC)
    # If we are building from a source distribution,
    # it also contains "INFO_SRC".
    # Similar, the export used for a release build already has the file.
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_SOURCE_DIR}/Docs/INFO_SRC ${INFO_SRC}
      )
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/INFO_SRC)
    # This is not the proper location, but who knows ...
    FILE(READ ${CMAKE_SOURCE_DIR}/INFO_SRC SOURCE_INFO)
    FILE(WRITE ${INFO_SRC} "${SOURCE_INFO}\n")
  ELSE()
    # This is a fall-back.
    FILE(WRITE ${INFO_SRC} "\nMySQL source ${VERSION}\n")
  ENDIF()
ENDMACRO(CREATE_INFO_SRC)


# This is for the "real" build, must be run again with each cmake run
# to make sure we report the current flags (not those of some previous run).

MACRO(CREATE_INFO_BIN)
  SET(INFO_BIN "Docs/INFO_BIN")
  FILE(WRITE ${INFO_BIN} "===== Information about the build process: =====\n")

  STRING(TIMESTAMP TMP_DATE "%Y-%m-%d %H:%M:%SZ" UTC)
  FILE(APPEND ${INFO_BIN} "Build was run at ${TMP_DATE}\n")

  FILE(APPEND ${INFO_BIN}
    "Build was done on ${CMAKE_HOST_SYSTEM} "
    "processor ${CMAKE_HOST_SYSTEM_PROCESSOR}\n"
    )

  FILE(APPEND ${INFO_BIN} "Build was done using cmake ${CMAKE_VERSION}\n\n")

  FILE(APPEND ${INFO_BIN} "===== CMAKE_GENERATOR =====\n")
  FILE(APPEND ${INFO_BIN} ${CMAKE_GENERATOR} "\n\n")

  FILE(APPEND ${INFO_BIN} "===== Feature flags used: =====\n")

  # -L List non-advanced cached variables.
  # -N View mode only.
  # Only load the cache. Do not actually run configure and generate steps.
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
    -N -L ${CMAKE_BINARY_DIR} OUTPUT_VARIABLE FEATURE_FLAGS)
  FILE(APPEND ${INFO_BIN} ${FEATURE_FLAGS} "\n")

  FILE(APPEND ${INFO_BIN} "===== Compiler flags used: =====\n")
  FILE(APPEND ${INFO_BIN} "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n")
  FILE(APPEND ${INFO_BIN} "Compiler: ${COMPILER_ID_AND_VERSION}\n")
  FILE(APPEND ${INFO_BIN} "COMPILE_DEFINITIONS: ${CWD_DEFINITIONS}\n")
  FILE(APPEND ${INFO_BIN} "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}\n")
  FILE(APPEND ${INFO_BIN} "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")

  STRING(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKEBT)
  FILE(APPEND ${INFO_BIN}
    "CMAKE_C_FLAGS_${CMAKEBT}: ${CMAKE_C_FLAGS_${CMAKEBT}}\n")
  FILE(APPEND ${INFO_BIN}
    "CMAKE_CXX_FLAGS_${CMAKEBT}: ${CMAKE_CXX_FLAGS_${CMAKEBT}}\n")

  IF(HAVE_BUILD_ID_SUPPORT AND NOT WITHOUT_SERVER)
    FILE(APPEND ${INFO_BIN} "\n===== BUILD ID =====\n")
    EXECUTE_PROCESS(COMMAND
      ${MYSQLD_EXECUTABLE} --no-defaults --help
      OUTPUT_VARIABLE mysqld_help
      RESULT_VARIABLE mysqld_help_result
      ERROR_VARIABLE mysqld_help_error
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    IF(mysqld_help_result)
      MESSAGE(FATAL_ERROR
        "mysqld --no-defaults --help failed: ${mysqld_help_error}")
    ENDIF()
    STRING(REPLACE "\n" ";" mysqld_help_list "${mysqld_help}")
    UNSET(BUILD_ID_FOUND)
    FOREACH(LINE ${mysqld_help_list})
      IF(LINE MATCHES "BuildID")
        FILE(APPEND ${INFO_BIN} "${LINE}\n")
        SET(BUILD_ID_FOUND 1)
        BREAK()
      ENDIF()
    ENDFOREACH()
    IF(NOT BUILD_ID_FOUND)
      MESSAGE(FATAL_ERROR "Could not find BuildID for mysqld")
    ENDIF()
  ENDIF()
  FILE(APPEND ${INFO_BIN} "===== EOF =====\n")
ENDMACRO(CREATE_INFO_BIN)
