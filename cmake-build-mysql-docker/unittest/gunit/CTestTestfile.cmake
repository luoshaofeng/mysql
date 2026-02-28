# CMake generated Testfile for 
# Source directory: /tmp/mysql/unittest/gunit
# Build directory: /tmp/mysql/cmake-build-mysql-docker/unittest/gunit
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(merge_small_tests "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/merge_small_tests-t")
set_tests_properties(merge_small_tests PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/CMakeLists.txt;357;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/CMakeLists.txt;0;")
add_test(merge_large_tests "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/merge_large_tests-t")
set_tests_properties(merge_large_tests PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/CMakeLists.txt;370;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/CMakeLists.txt;0;")
add_test(rpl_channel_credentials "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/rpl_channel_credentials-t")
set_tests_properties(rpl_channel_credentials PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/CMakeLists.txt;428;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/CMakeLists.txt;0;")
add_test(rpl_commit_order_queue "/tmp/mysql/cmake-build-mysql-docker/runtime_output_directory/rpl_commit_order_queue-t")
set_tests_properties(rpl_commit_order_queue PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mysql/cmake/mysql_add_executable.cmake;221;ADD_TEST;/tmp/mysql/unittest/gunit/CMakeLists.txt;438;MYSQL_ADD_EXECUTABLE;/tmp/mysql/unittest/gunit/CMakeLists.txt;0;")
subdirs("ddl_rewriter")
subdirs("innodb")
subdirs("keyring")
subdirs("components/mysql_server")
subdirs("components/keyring_common")
subdirs("xplugin")
subdirs("group_replication")
subdirs("libmysqlgcs")
subdirs("temptable")
subdirs("binlogevents")
subdirs("memory")
subdirs("containers")
subdirs("locks")
subdirs("changestreams")
