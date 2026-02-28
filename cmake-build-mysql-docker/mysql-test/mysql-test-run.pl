#!/usr/bin/perl
# Call mtr in out-of-source build
$ENV{MTR_BINDIR} = '/tmp/mysql/cmake-build-mysql-docker';
chdir('/tmp/mysql/mysql-test');
exit(system($^X, '/tmp/mysql/mysql-test/mysql-test-run.pl', @ARGV) >> 8);
