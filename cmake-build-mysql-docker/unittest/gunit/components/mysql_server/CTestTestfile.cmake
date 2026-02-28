# CMake generated Testfile for 
# Source directory: /tmp/mysql/unittest/gunit/components/mysql_server
# Build directory: /tmp/mysql/cmake-build-mysql-docker/unittest/gunit/components/mysql_server
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(registry "/tmp/mysql/cmake-build-mysql-docker/plugin_output_directory/registry-t")
set_tests_properties(registry PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;37;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;0;")
add_test(dynamic_loader "/tmp/mysql/cmake-build-mysql-docker/plugin_output_directory/dynamic_loader-t")
set_tests_properties(dynamic_loader PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;37;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;0;")
add_test(minimal_chassis-t "/tmp/mysql/cmake-build-mysql-docker/plugin_output_directory/minimal_chassis-t")
set_tests_properties(minimal_chassis-t PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;46;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;0;")
add_test(minimal_chassis_test_driver-t "/tmp/mysql/cmake-build-mysql-docker/plugin_output_directory/minimal_chassis_test_driver-t")
set_tests_properties(minimal_chassis_test_driver-t PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;55;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;0;")
add_test(reference_cache-t "/tmp/mysql/cmake-build-mysql-docker/plugin_output_directory/reference_cache-t")
set_tests_properties(reference_cache-t PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;97;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/components/mysql_server/CMakeLists.txt;0;")
