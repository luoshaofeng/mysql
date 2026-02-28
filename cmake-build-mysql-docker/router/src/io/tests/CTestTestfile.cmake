# CMake generated Testfile for 
# Source directory: /tmp/mysql/router/src/io/tests
# Build directory: /tmp/mysql/cmake-build-mysql-docker/router/src/io/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(routertest_io_io_plugin "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/routertest_io_io_plugin")
set_tests_properties(routertest_io_io_plugin PROPERTIES  ENVIRONMENT "CMAKE_SOURCE_DIR=/tmp/mysql/router;CMAKE_BINARY_DIR=/tmp/mysql/cmake-build-mysql-docker/router;LD_LIBRARY_PATH=;DYLD_LIBRARY_PATH=;TSAN_OPTIONS=suppressions=/tmp/mysql/router/tsan.supp;" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/router/cmake/testing.cmake;120;MYSQL_ADD_EXECUTABLE;/tmp/mysql/router/cmake/testing.cmake;53;_ADD_TEST_FILE;/tmp/mysql/router/cmake/testing.cmake;39;ADD_ROUTER_TEST_FILE;/tmp/mysql/router/src/io/tests/CMakeLists.txt;27;add_test_file;/tmp/mysql/router/src/io/tests/CMakeLists.txt;0;")
add_test(routertest_io_io_component "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/routertest_io_io_component")
set_tests_properties(routertest_io_io_component PROPERTIES  ENVIRONMENT "CMAKE_SOURCE_DIR=/tmp/mysql/router;CMAKE_BINARY_DIR=/tmp/mysql/cmake-build-mysql-docker/router;LD_LIBRARY_PATH=;DYLD_LIBRARY_PATH=;TSAN_OPTIONS=suppressions=/tmp/mysql/router/tsan.supp;" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/router/cmake/testing.cmake;120;MYSQL_ADD_EXECUTABLE;/tmp/mysql/router/cmake/testing.cmake;53;_ADD_TEST_FILE;/tmp/mysql/router/cmake/testing.cmake;39;ADD_ROUTER_TEST_FILE;/tmp/mysql/router/src/io/tests/CMakeLists.txt;35;add_test_file;/tmp/mysql/router/src/io/tests/CMakeLists.txt;0;")
