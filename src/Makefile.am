AM_CFLAGS       = -W -Wall -Wextra -funsigned-char
AM_CPPFLAGS     = -I$(top_srcdir)
EXTRA_DIST      = advent.desktop.in
TXT_FILES       = advent1.txt advent2.txt advent3.txt advent4.txt

desktopdir      = $(datadir)/applications
desktop_DATA    = advent.desktop

bin_PROGRAMS    = advent
noinst_PROGRAMS = advent0

advent_SOURCES  = advent.c database.c english.c itverb.c turn.c verb.c   \
		  advcave.h advdec.h advdef.h advent.h advword.h proto.h \
		  advent1.h advent2.h advent3.h advent4.h advtext.h

advent0_SOURCES = advent0.c

advtext.h advent1.h advent2.h advent3.h advent4.h: advent0 $(TXT_FILES)
	./advent0
