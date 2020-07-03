AC_INIT([advent], [4.1-dev], [https://github.com/troglobit/advent4/issues])
AC_CONFIG_AUX_DIR(aux)
AM_INIT_AUTOMAKE([1.11 foreign])

AC_CONFIG_SRCDIR([src/advent.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

AC_PROG_CC
AC_PROG_INSTALL

AC_HEADER_STDC

AC_ARG_ENABLE([builtin],
	AS_HELP_STRING([--disable-builtin], [disable built-in .txt files]),,
	enable_builtin=yes)

AS_IF([test "x$enable_builtin" != "xno"],
	AC_DEFINE([BUILTIN], 1, [Enable built-in .txt files (default)]))

AC_OUTPUT
