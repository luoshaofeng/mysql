# CMake generated Testfile for 
# Source directory: /tmp/mysql/storage/ndb
# Build directory: /tmp/mysql/cmake-build-mysql-docker/storage/ndb
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ndb_bitmap-t "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/ndb_bitmap-t")
set_tests_properties(ndb_bitmap-t PROPERTIES  LABELS "NDB" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/storage/ndb/cmake/ndb_add_test.cmake;49;MYSQL_ADD_EXECUTABLE;/tmp/mysql/storage/ndb/CMakeLists.txt;141;NDB_ADD_TEST;/tmp/mysql/storage/ndb/CMakeLists.txt;0;")
add_test(ndb_blobs_buffer-t "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/ndb_blobs_buffer-t")
set_tests_properties(ndb_blobs_buffer-t PROPERTIES  LABELS "NDB" _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/storage/ndb/cmake/ndb_add_test.cmake;49;MYSQL_ADD_EXECUTABLE;/tmp/mysql/storage/ndb/CMakeLists.txt;142;NDB_ADD_TEST;/tmp/mysql/storage/ndb/CMakeLists.txt;0;")
subdirs("include")
subdirs("src")
