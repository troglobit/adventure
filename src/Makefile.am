AM_CFLAGS       = -W -Wall -Wextra -funsigned-char
AM_CPPFLAGS     = -I$(top_srcdir)
EXTRA_DIST      = advent.desktop.in

desktopdir      = $(datadir)/applications
desktop_DATA    = advent.desktop

bin_PROGRAMS    = advent
noinst_PROGRAMS = advent0

advent_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"
advent_SOURCES  = advent.c database.c english.c itverb.c turn.c verb.c   \
		  advcave.h advdec.h advdef.h advent.h advword.h proto.h \
		  advent1.h advent2.h advent3.h advent4.h advtext.h

if GAMEDATA
gamedir         = $(pkgdatadir)
game_DATA       = advent1.txt advent2.txt advent3.txt advent4.txt
endif

# Run advent0 if you change the .txt files.  It updates the corresponding
# .h files and the advtext.h index.
advent0_SOURCES = advent0.c
