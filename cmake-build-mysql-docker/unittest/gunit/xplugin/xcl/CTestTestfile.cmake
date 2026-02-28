# CMake generated Testfile for 
# Source directory: /tmp/mysql/unittest/gunit/xplugin/xcl
# Build directory: /tmp/mysql/cmake-build-mysql-docker/unittest/gunit/xplugin/xcl
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(xclient "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/xclient_unit_tests")
set_tests_properties(xclient PROPERTIES  TIMEOUT "180" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/xplugin/xcl/CMakeLists.txt;53;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/xplugin/xcl/CMakeLists.txt;0;")
