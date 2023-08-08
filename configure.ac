AC_INIT([advent],[4.2],[https://github.com/troglobit/adventure/issues])
AC_CONFIG_AUX_DIR(aux)
AM_INIT_AUTOMAKE([1.11 foreign])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/advent.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile src/advent.desktop])

AC_PROG_CC
AC_PROG_INSTALL

AC_ARG_ENABLE([builtin],
	AS_HELP_STRING([--disable-builtin], [disable built-in game data .txt files]),,
	enable_builtin=yes)

AC_ARG_WITH([savedir],
	[AS_HELP_STRING([--with-savedir=DIR], [use global save directory, default $HOME])],
	[with_savedir=$withval], [with_savedir=no])

AS_IF([test "x$enable_builtin" != "xno"],
	AC_DEFINE([BUILTIN], 1, [Enable built-in .txt files (default)]))
AM_CONDITIONAL(GAMEDATA, [test "x$enable_builtin" != "xyes"])

AS_IF([test "x$with_savedir" != "xno"], [
	AS_IF([test "x$with_savedir" = "xyes"], [with_savedir=${localstatedir}/games])
	AC_DEFINE([SAVEDIR], "$with_savedir", [Global savedir instead of $HOME])])

AS_IF([test "x$program_suffix" != "xNONE"],
	[SUFFIX=$program_suffix], [SUFFIX=])
AC_SUBST(SUFFIX)

# Expand $sbindir early, into $SBINDIR, for systemd unit file
# NOTE: This does *not* take prefix/exec_prefix override at "make
#       install" into account, unfortunately.
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'
DATADIR=`eval echo $datadir`
GAMEDATADIR=`eval echo $DATADIR/$PACKAGE_NAME`
AC_OUTPUT

cat <<EOF

------------------ Summary ------------------
 $PACKAGE_NAME version $PACKAGE_VERSION
  Prefix................: $prefix
  Game datadir..........: $GAMEDATADIR
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
