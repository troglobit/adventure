Colossal Cave Adventure
=======================

This is a port of the original Colossal Cave text adventure to UNIX.


Build & Install
---------------

This library was initially built for and developed on GNU/Linux systems
as a light weight utility library, these days NetBSD should also work.

    ./configure --prefix=/usr --localstatedir=/var
    make -j5
    sudo make install-strip

**Note:** When checking out code from GIT, use <kbd>./autogen.sh</kbd>
to generate a `configure` script.  It is a generated file and otherwise
only included in released tarballs.  This is the only time you need the
autoconf and automake tools.


Origin & References
-------------------

The original Fortran code was by William Crowther, with major features
added by Don Woods.  Conversion to BDS C by J. R. Jaeger and UNIX<TM>
standardization by Jerry D. Pohl. The last port to QNX 4 and bug fixes
by James Lummel.  This last version should work without any change on
HP-UX, Sun Solaris, *BSD and GNU/Linux systems.

The actual license of this program is unknown, although most people
agree that it is likely to be considered to be under the public domain.

See the files in the doc/ directory for the original documentation,
including cheat codes and some history.

The original code, uncovered:
- http://groups.google.com/group/rec.arts.int-fiction/browse_thread/thread/607acaf1a279d4dd/9181d75148500b30

The Wikipedia entry:

- http://en.wikipedia.org/wiki/Colossal_Cave_Adventure

Other writings on this cult classic:

- http://www.wurb.com/if/game/1
- http://www.rickadams.org/adventure/a_history.html

 -- Compiled on July 26th, 2009 by Joachim Nilsson <troglobit@gmail.com>
