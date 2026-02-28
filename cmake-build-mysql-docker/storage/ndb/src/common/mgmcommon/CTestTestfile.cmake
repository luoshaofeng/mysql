# CMake generated Testfile for 
# Source directory: /tmp/mysql/storage/ndb/src/common/mgmcommon
# Build directory: /tmp/mysql/cmake-build-mysql-docker/storage/ndb/src/common/mgmcommon
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(thr_config-t "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/thr_config-t")
set_tests_properties(thr_config-t PROPERTIES  LABELS "NDB" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/storage/ndb/cmake/ndb_add_test.cmake;49;MYSQL_ADD_EXECUTABLE;/tmp/mysql/storage/ndb/src/common/mgmcommon/CMakeLists.txt;49;NDB_ADD_TEST;/tmp/mysql/storage/ndb/src/common/mgmcommon/CMakeLists.txt;0;")
add_test(InitConfigFileParser-t "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/InitConfigFileParser-t")
set_tests_properties(InitConfigFileParser-t PROPERTIES  LABELS "NDB" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/storage/ndb/cmake/ndb_add_test.cmake;49;MYSQL_ADD_EXECUTABLE;/tmp/mysql/storage/ndb/src/common/mgmcommon/CMakeLists.txt;50;NDB_ADD_TEST;/tmp/mysql/storage/ndb/src/common/mgmcommon/CMakeLists.txt;0;")
