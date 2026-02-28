# CMake generated Testfile for 
# Source directory: /tmp/mysql/router/src/keepalive/tests
# Build directory: /tmp/mysql/cmake-build-mysql-docker/router/src/keepalive/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(routertest_harness_plugin_keepalive "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/routertest_harness_plugin_keepalive")
set_tests_properties(routertest_harness_plugin_keepalive PROPERTIES  ENVIRONMENT "CMAKE_SOURCE_DIR=/tmp/mysql/router;CMAKE_BINARY_DIR=/tmp/mysql/cmake-build-mysql-docker/router;LD_LIBRARY_PATH=;DYLD_LIBRARY_PATH=;TSAN_OPTIONS=suppressions=/tmp/mysql/router/tsan.supp;" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/router/cmake/testing.cmake;120;MYSQL_ADD_EXECUTABLE;/tmp/mysql/router/cmake/testing.cmake;53;_ADD_TEST_FILE;/tmp/mysql/router/cmake/testing.cmake;39;ADD_ROUTER_TEST_FILE;/tmp/mysql/router/src/keepalive/tests/CMakeLists.txt;31;ADD_TEST_FILE;/tmp/mysql/router/src/keepalive/tests/CMakeLists.txt;0;")
