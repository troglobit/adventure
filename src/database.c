
/*	program DATABASE.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/

#include <stdio.h> /* drv = 1.1st file 2.def 3.A	*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "advent.h"
#include "advdec.h"

#ifdef BUILTIN
#include "advent1.h"
#include "advent2.h"
#include "advent3.h"
#include "advent4.h"
#endif


/*
	Routine to fill travel array for a given location
*/
void gettrav(int loc)
{
	char atrav[240] = { 0 }; /* max length of any cave pos = 144 bytes */
	char *aptr;
	long t;
	int i;

	if (loc <= 0 || loc >= (int)cavesz)
		bug(42);
	if (strlen(cave[loc - 1]) > sizeof(atrav))
		bug(43);

	strcpy(atrav, cave[loc - 1]);
	while ((aptr = strrchr(atrav, ',')))
		*aptr = '\0'; /* terminate substring	*/
	aptr = &atrav[0];
	for (i = 0; i < MAXTRAV - 1; ++i) {
		t = atol(aptr); /* convert to long int	*/
		travel[i].tcond = (int)(t % 1000L);
		t /= 1000L;
		travel[i].tverb = (int)(t % 1000L);
		t /= 1000L;
		travel[i].tdest = (int)(t % 1000L);
		while (*(aptr++))
			; /* to next substring	*/
		if (!(*aptr)) {
			travel[++i].tdest = -1; /* end of array	*/
			if (dbugflg) {
				for (i = 0; i < MAXTRAV; ++i) {
					printf("cave[%d] = %d %d %d\n", loc,
					       travel[i].tdest, travel[i].tverb, travel[i].tcond);
				}
			}
			return; /* terminate for loop	*/
		}
	}
	bug(33);
}
#ifndef BUILTIN
/*
	Function to scan a file up to a specified
	point and either print or return a string.
*/
int rdupto(FILE *fdi, char uptoc, char print, char *string)
{
	int c;

	while ((c = fgetc(fdi)) != uptoc) {
		if (c == EOF)
			return 0;
		if (c == '\r')
			continue;
		if (print)
			fputc(c, stdout);
		else
			*string++ = c;
	}
	if (!print)
		*string = '\0';
	return 1;
}

/*
	Function to read a file skipping
	a given character a specified number
	of times, with or without repositioning
	the file.
*/
void rdskip(FILE *fdi, char skipc, int n, char rewind)
{
	int c;

	if (rewind)
		if (fseek(fdi, 0, SEEK_SET) == -1)
			bug(31);
	while (n--) {
		while ((c = fgetc(fdi)) != skipc) {
			if (c == EOF)
				bug(32);
		}
	}
}
#endif

/*
	Routine to request a yes or no answer to a question.
*/
int yes(int msg1, int msg2, int msg3)
{
	char answer[80];

	if (msg1)
		rspeak(msg1);
	fputs("> ", stdout);
	fflush(stdout);
	if (NULL == fgets(answer, 80, stdin))
		exit(0);
	if (tolower(answer[0]) == 'n') {
		if (msg3)
			rspeak(msg3);
		return 0;
	}
	if (msg2)
		rspeak(msg2);
	return 1;
}

/*
	Print a location description from "advent4.txt"
*/
void rspeak(int msg)
{
#ifdef BUILTIN
	fputs(adventtxt4[msg - 1], stdout);
#else
	if (msg == 54)
		printf("ok.\n");
	else {
		if (dbugflg)
			printf("Seek loc msg #%d @ %ld\n", msg, idx4[msg]);
		fseek(fd4, idx4[msg - 1], SEEK_SET);
		rdupto(fd4, '#', 1, 0);
	}
#endif
	return;
}

/*
	Print an item message for a given state from "advent3.txt"
*/
void pspeak(int item, int state)
{
#ifdef BUILTIN
	const char *p;

	if (item > 64) {
		bug(40);
		return;
	}

	p = adventtxt3[item - 1];
	if (p == NULL)
		bug(31);
	else {
		int c;
		int n = state + 2;

		while (n--) {
			while ((c = *p++) != '/') {
				if (c == '\0')
					bug(32);
			}
		}
		for (n = 0; p[n] != '\0' && p[n] != '/'; n++)
			putchar(p[n]);
	}
#else
	fseek(fd3, idx3[item - 1], SEEK_SET);
	rdskip(fd3, '/', state + 2, 0);
	rdupto(fd3, '/', 1, 0);
#endif
}

/*
	Print a long location description from "advent1.txt"
*/
void desclg(int loc)
{
#ifdef BUILTIN
	fputs(adventtxt1[loc - 1], stdout);
#else
	fseek(fd1, idx1[loc - 1], SEEK_SET);
	rdupto(fd1, '#', 1, 0);
#endif
}

/*
	Print a short location description from "advent2.txt"
*/
void descsh(int loc)
{
#ifdef BUILTIN
	fputs(adventtxt2[loc - 1], stdout);
#else
	fseek(fd2, idx2[loc - 1], SEEK_SET);
	rdupto(fd2, '#', 1, 0);
#endif
}

/*
	look-up vocabulary word in lex-ordered table.  words may have
	two entries with different codes. if minimum acceptable value
	= 0, then return minimum of different codes.  last word CANNOT
	have two entries(due to binary sort).
	word is the word to look up.
	val  is the minimum acceptable value,
		if != 0 return %1000
*/
int vocab(char *word, int val)
{
	int v1, v2;

	if ((v1 = binary(word, wc, MAXWC)) >= 0) {
		v2 = binary(word, wc, MAXWC - 1);
		if (v2 < 0)
			v2 = v1;
		if (!val)
			return wc[v1].acode < wc[v2].acode ? wc[v1].acode : wc[v2].acode;
		if (val <= wc[v1].acode)
			return wc[v1].acode % 1000;
		else if (val <= wc[v2].acode)
			return wc[v2].acode % 1000;
		else
			return -1;
	} else
		return -1;
}

int binary(char *w, struct wac wctable[], int maxwc)
{
	int lo, mid, hi, check;

	lo = 0;
	hi = maxwc - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		if ((check = strcmp(w, wctable[mid].aword)) < 0)
			hi = mid - 1;
		else if (check > 0)
			lo = mid + 1;
		else
			return mid;
	}
	return -1;
}

/*
	Utility Routines
*/

/*
	Routine to test for darkness
*/
int dark(void)
{
	return !(cond[loc] & LIGHT) && (!prop[LAMP] || !here(LAMP));
}

/*
	Routine to tell if an item is present.
*/
int here(int item)
{
	return place[item] == loc || toting(item);
}

/*
	Routine to tell if an item is being carried.
*/
int toting(int item)
{
	return place[item] == -1;
}

/*
	Routine to tell if a location causes
	a forced move.
*/
int forced(int atloc)
{
	return cond[atloc] == 2;
}

/*
	Routine true x% of the time.
*/
int pct(int x)
{
	return rand() % 100 < x;
}

/*
	Routine to tell if player is on
	either side of a two sided object.
*/
int at(int item)
{
	return place[item] == loc || fixed[item] == loc;
}

/*
	Routine to destroy an object
*/
void dstroy(int obj)
{
	move(obj, 0);
}

/*
	Routine to move an object
*/
void move(int obj, int where)
{
	int from;

	from = (obj < MAXOBJ) ? place[obj] : fixed[obj - MAXOBJ];
	if (from > 0 && from <= 300)
		carry(obj, from);
	drop(obj, where);
}

/*
	Juggle an object
	currently a no-op
*/
void juggle(int loc)
{
	(void)loc;
}

/*
	Routine to carry an object
*/
void carry(int obj, int where)
{
	(void)where;

	if (obj < MAXOBJ) {
		if (place[obj] == -1)
			return;
		place[obj] = -1;
		++holding;
	}
}

/*
	Routine to drop an object
*/
void drop(int obj, int where)
{
	if (obj < MAXOBJ) {
		if (place[obj] == -1)
			--holding;
		place[obj] = where;
	} else
		fixed[obj - MAXOBJ] = where;
}

/*
	routine to move an object and return a
	value used to set the negated prop values
	for the repository.
*/
int put(int obj, int where, int pval)
{
	move(obj, where);
	return (-1) - pval;
}
/*
	Routine to check for presence
	of dwarves..
*/
int dcheck(void)
{
	int i;

	for (i = 1; i < (DWARFMAX - 1); ++i){
		if (dloc[i] == loc)
			return i;
	}
	return 0;
}

/*
	Determine liquid in the bottle
*/
int liq(void)
{
	int i, j;
	i = prop[BOTTLE];
	j = -1 - i;
	return liq2(i > j ? i : j);
}

/*
	Determine liquid at a location
*/
int liqloc(int loc)
{
	if (cond[loc] & LIQUID)
		return liq2(cond[loc] & WATOIL);

	return liq2(1);
}

/*
	Convert  0 to WATER
		 1 to nothing
		 2 to OIL
*/
int liq2(int pbottle)
{
	return (1 - pbottle) * WATER + (pbottle >> 1) * (WATER + OIL);
}

/*
	Fatal error routine
*/
void bug(int n)
{
	printf("Fatal error number %d\n", n);
	exit(-1);
}
