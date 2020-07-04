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

AC_ARG_WITH([savedir],
	[AS_HELP_STRING([--with-savedir=DIR], [use global save directory, default $HOME])],
	[with_savedir=$withval], [with_savedir=no])

AS_IF([test "x$enable_builtin" != "xno"],
	AC_DEFINE([BUILTIN], 1, [Enable built-in .txt files (default)]))

AS_IF([test "x$with_savedir" != "xno"], [
	AS_IF([test "x$with_savedir" = "xyes"], [with_savedir=${localstatedir}/games])
	AC_DEFINE([SAVEDIR], "$with_savedir", [Global savedir instead of $HOME])])

AC_OUTPUT

cat <<EOF

------------------ Summary ------------------
 $PACKAGE_NAME version $PACKAGE_VERSION
  Prefix................: $prefix
  Localstatedir.........: `eval echo $localstatedir`
  C Compiler............: $CC $CFLAGS $CPPFLAGS $LDFLAGS $LIBS

Optional features:
  Built-in .txt files...: $enable_builtin
  Global save directory.: $with_savedir

------------- Compiler version --------------
$($CC --version || true)
---------------------------------------------

Check the above options and compile with:
 ${MAKE-make}

EOF
