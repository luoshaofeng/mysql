/*
   Copyright (c) 2010, 2025, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is designed to work with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have either included with
   the program or referenced in the documentation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#define HAVE_POSIX_FALLOCATE 1
#define HAVE_POSIX_MEMALIGN 1
#define HAVE_CLOCK_GETTIME 1
#define HAVE_NANOSLEEP 1
#define HAVE_PTHREAD_CONDATTR_SETCLOCK 1
#define HAVE_PTHREAD_SELF 1
#define HAVE_SCHED_GET_PRIORITY_MIN 1
#define HAVE_SCHED_GET_PRIORITY_MAX 1
/* #undef HAVE_SCHED_SETAFFINTIY */
#define HAVE_SCHED_SETSCHEDULER 1
/* #undef HAVE_PROCESSOR_BIND */
#define HAVE_EPOLL_CREATE 1
#define HAVE_MEMALIGN 1
#define HAVE_SYSCONF 1
/* #undef HAVE_DIRECTIO */
/* #undef HAVE_ATOMIC_SWAP_32 */
/* #undef HAVE_MAC_OS_X_THREAD_INFO */
#define HAVE_LINUX_SCHEDULING 1
/* #undef HAVE_CPUSET_SETAFFINITY */
#define HAVE_SETPRIORITY 1
/* #undef HAVE_PRIOCNTL */
/* #undef HAVE_PROCESSOR_AFFINITY */
/* #undef HAVE_SOLARIS_AFFINITY */
#define HAVE_LINUX_FUTEX 1
/* #undef HAVE_ATOMIC_H */
/* #undef HAVE_PROCESSTOPOLOGYAPI_H */
/* #undef HAVE_PROCESSTHREADSAPI_H */
#define HAVE_NCURSESW_CURSES_H 1
/* #undef HAVE_NCURSESW_H */
#define HAVE_NCURSES_H 1
/* #undef HAVE_NCURSES_CURSES_H */
/* #undef NDB_PORT */
#define HAVE_MLOCK 1
#define HAVE_FFS 1
#define HAVE___BUILTIN_FFS 1
#define HAVE___BUILTIN_CTZ 1
#define HAVE___BUILTIN_CLZ 1
/* #undef HAVE__BITSCANFORWARD */
/* #undef HAVE__BITSCANREVERSE */
#define HAVE_PTHREAD_MUTEXATTR_INIT 1
#define HAVE_PTHREAD_MUTEXATTR_SETTYPE 1
#define HAVE_PTHREAD_MUTEXATTR_SETPSHARED 1
#define HAVE_PTHREAD_SETSCHEDPARAM 1
#define HAVE_EXPLICIT_BZERO 1
/* #undef HAVE_MEMSET_S */
#define HAVE_POLL_H 1
