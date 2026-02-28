# CMake generated Testfile for 
# Source directory: /tmp/mysql/unittest/gunit/innodb
# Build directory: /tmp/mysql/cmake-build-mysql-docker/unittest/gunit/innodb
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(merge_innodb_tests-t "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/merge_innodb_tests-t")
set_tests_properties(merge_innodb_tests-t PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/innodb/CMakeLists.txt;81;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/innodb/CMakeLists.txt;0;")
subdirs("lob")
