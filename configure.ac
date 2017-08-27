#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libjco], [0.1], [shilling.jake@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

AC_CONFIG_SRCDIR([include/jco/jco.h])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])
AC_SEARCH_LIBS([sqrt],[m])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([asprintf])

AC_OUTPUT([Makefile
	   src/Makefile
	   test/Makefile])
