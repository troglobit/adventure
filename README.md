Colossal Cave Adventure
=======================

This is the real, full-blown adventure program written by Will Crowther
and Don Woods at SAIL in the early seventies.  This Public Domain
version has been translated from BDS C to CII by Jerry D. Pohl,
including UNIX standardization.

Before you begin, a few suggestions:

1. Make a map.  There are two common ways to handle this. You can use a
   piece of butcher paper and a pencil and sketch in all the rooms as
   circles or whatever with directions marked for the lines between
   rooms.  The other way to make a map is to make a word table with the
   room names down one side along with numbers you've assigned each
   room.  Along the top of the table you put N, NW, W, SW, S, SE, E and
   so on.  (don't forget up and down).  Then you can tell at a glance
   which directions you have tried and haven't tried.

2. Save the game every half hour or so (unless you have spent a lot of
   moves getting nowhere).

3. Take your time.  I've seen major gamers spend 6 months working on
   this one.  They were spending 2 or 3 hours a day and they weren't
   beginners!

Try everything!

Sometimes you will have to go back to a previous location to be able to
do something.  The game parser uses simple two word (verb-noun) commands
(after all, this is the first text adventure game. Infocom wasn't to be
for a while yet!)


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
- http://www.spitenet.com/cave/
- http://advent.jenandcal.familyds.org/
- http://www.rickadams.org/adventure/a_history.html
- https://github.com/Quuxplusone/Advent

 -- Compiled on July 26th, 2009 and updated August 4th 2023
    by Joachim Wiberg <troglobit@gmail.com>
