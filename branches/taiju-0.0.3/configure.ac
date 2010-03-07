#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([taiju], [0.0.3], [syata@acm.org])
AC_CONFIG_SRCDIR([include/taiju/int-types.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL

AC_LANG_CPLUSPLUS
AC_C_INLINE

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL

AC_CHECK_HEADERS([fcntl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([munmap])

AC_FUNC_MMAP

AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile lib/Makefile tool/Makefile test/Makefile])
AC_OUTPUT
