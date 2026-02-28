# Install script for directory: /tmp/mysql/man

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xManPagesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/tmp/mysql/man/comp_err.1"
    "/tmp/mysql/man/ibd2sdi.1"
    "/tmp/mysql/man/innochecksum.1"
    "/tmp/mysql/man/lz4_decompress.1"
    "/tmp/mysql/man/my_print_defaults.1"
    "/tmp/mysql/man/myisam_ftdump.1"
    "/tmp/mysql/man/myisamchk.1"
    "/tmp/mysql/man/myisamlog.1"
    "/tmp/mysql/man/myisampack.1"
    "/tmp/mysql/man/mysql.1"
    "/tmp/mysql/man/mysql_config.1"
    "/tmp/mysql/man/mysql_config_editor.1"
    "/tmp/mysql/man/mysql_secure_installation.1"
    "/tmp/mysql/man/mysql_ssl_rsa_setup.1"
    "/tmp/mysql/man/mysql_tzinfo_to_sql.1"
    "/tmp/mysql/man/mysql_upgrade.1"
    "/tmp/mysql/man/mysqladmin.1"
    "/tmp/mysql/man/mysqlbinlog.1"
    "/tmp/mysql/man/mysqlcheck.1"
    "/tmp/mysql/man/mysqldump.1"
    "/tmp/mysql/man/mysqldumpslow.1"
    "/tmp/mysql/man/mysqlimport.1"
    "/tmp/mysql/man/mysqlman.1"
    "/tmp/mysql/man/mysqlpump.1"
    "/tmp/mysql/man/mysqlshow.1"
    "/tmp/mysql/man/mysqlslap.1"
    "/tmp/mysql/man/perror.1"
    "/tmp/mysql/man/zlib_decompress.1"
    "/tmp/mysql/man/mysql.server.1"
    "/tmp/mysql/man/mysqld_multi.1"
    "/tmp/mysql/man/mysqld_safe.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xManPagesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man8" TYPE FILE FILES "/tmp/mysql/man/mysqld.8")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xManPagesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/tmp/mysql/man/mysqlrouter.1"
    "/tmp/mysql/man/mysqlrouter_passwd.1"
    "/tmp/mysql/man/mysqlrouter_plugin_info.1"
    )
endif()

