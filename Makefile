# Colossal Cave Adventure Makefile

# HEADERS= advcave.h advdec.h advdef.h advent.h advtext.h advword.h proto.h
HEADERS= advcave.h advdec.h advdef.h advent.h advword.h proto.h
SRC= advent.c database.c english.c itverb.c turn.c verb.c
OBJ= advent.o database.o english.o itverb.o turn.o verb.o
DATA= advent1.txt advent2.txt advent3.txt advent4.txt

CFLAGS +=  -D__QNX__ -funsigned-char
# LDFLAGS=
# CC= cc

all: advent4

advent4: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

advent0: advent0.o
	cc -o advent0 $(CFLAGS) $(LDFLAGS) advent0.o

romfs:
	$(ROMFSINST) /bin/advent4
	$(ROMFSINST) -e CONFIG_USER_GAMES_ADVENT4 \
		-a "advent4 stream tcp nowait root /bin/advent4" /etc/inetd.conf

clean:
	rm -f *.o *.err advent4 advent4.elf advent4.gdb
#	rm -f *.o *.err advent advent0 advtext.h

install:
	cp -f advent /usr/local/bin
	chmod 0644 $(DATA)
	cp -f $(DATA) /usr/local/lib/games

advtext.h: advent0 $(DATA)
	advent0

advent.o: advent.c $(HEADERS)
database.o: database.c $(HEADERS)
english.o: english.c $(HEADERS)
itverb.o: itverb.c $(HEADERS)
turn.o: turn.c $(HEADERS)
verb.o: verb.c $(HEADERS)

