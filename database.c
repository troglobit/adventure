
/*	program DATABASE.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"
#include	"advdec.h"

#ifdef EMBED
#include "advent1.h"
#include "advent2.h"
#include "advent3.h"
#include "advent4.h"
#endif

#ifndef __QNX__
extern	long	atoi();
extern	int	fgetc();
extern	char	*fgets();
extern	int	fputc();
extern	long	fseek();
extern	char	*rindex();
extern	char	*strcpy();
extern	int	tolower();
#else
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define atoi atol
#define rindex strrchr
#endif


/*
	Routine to fill travel array for a given location
*/
gettrav(loc)
int	loc;
{
	int	i;
	long	t;
	char	atrav[256], *aptr;

	strcpy(atrav, cave[loc - 1]);
	while (aptr = rindex(atrav, ','))
		*aptr = '\0';		/* terminate substring	*/
	aptr = &atrav[0];
	for (i = 0; i < MAXTRAV; ++i) {
		t = atoi(aptr);		/* convert to long int	*/
		travel[i].tcond = (int) (t % 1000L);
		t /= 1000L;
		travel[i].tverb = (int) (t % 1000L);
		t /= 1000L;
		travel[i].tdest = (int) (t % 1000L);
		while (*(aptr++));		/* to next substring	*/
		if (!(*aptr)) {
		    travel[++i].tdest = -1;/* end of array	*/
		    if (dbugflg)
			for (i = 0; i < MAXTRAV; ++i)
				printf("cave[%d] = %d %d %d\n", \
				loc, travel[i].tdest, \
				travel[i].tverb, travel[i].tcond);
		    return;		/* terminate for loop	*/
		}
	}
	bug(33);
}
#ifndef EMBED
/*
	Function to scan a file up to a specified
	point and either print or return a string.
*/
rdupto(fdi, uptoc, print, string)
FILE	*fdi;
char	uptoc, print, *string;
{
	int	c;

	while ((c = fgetc(fdi)) != uptoc) {
		if (c == EOF)
			return(0);
		if (c == '\r')
			continue;
		if (print)
			fputc(c, stdout);
		else
			*string++ = c;
	}
	if (!print)
		*string = '\0';
	return(1);
}

/*
	Function to read a file skipping
	a given character a specified number
	of times, with or without repositioning
	the file.
*/
rdskip(fdi, skipc, n, rewind)
FILE	*fdi;
char	skipc, rewind;
int	n;
{
	int	c;

	if (rewind)
		if (fseek(fdi, 0, 0) == -1)
			bug(31);
	while (n--)
		while ((c = fgetc(fdi)) != skipc)
			if (c == EOF)
				bug(32);
}
#endif

/*
	Routine to request a yes or no answer to a question.
*/
yes(msg1, msg2, msg3)
int	msg1, msg2, msg3;
{
	char	answer[80];

	if (msg1)
		rspeak(msg1);
	fputc('>', stdout);
	fflush(stdout);
	if (NULL == fgets(answer, 80, stdin))
		exit(0);
	if (tolower(answer[0]) == 'n') {
		if (msg3)
			rspeak(msg3);
		return(0);
	}
	if (msg2)
		rspeak(msg2);
	return(1);
}

/*
	Print a location description from "advent4.txt"
*/
rspeak(msg)
int	msg;
{
#ifdef EMBED
	fputs(adventtxt4[msg-1], stdout);
#else
	if (msg == 54)
		printf("ok.\n");
	else {
		if (dbugflg)
		    printf("Seek loc msg #%d @ %ld\n", msg, idx4[msg]);
		fseek(fd4, idx4[msg - 1], 0);
		rdupto(fd4, '#', 1, 0);
	}
#endif
	return;
}

/*
	Print an item message for a given state from "advent3.txt"
*/
pspeak(item, state)
int	item, state;
{
#ifdef EMBED
	const char *p;
	p = adventtxt3[item-1];
	if (p == NULL) bug(31);
	else {
		int	c;
		int     n = state+2;

		while (n--)
			while ((c = *p++) != '/')
				if (c == '\0')
					bug(32);
		for (n=0; p[n] != '\0' && p[n] != '/'; n++)
			putchar(p[n]);
	}
#else
	fseek(fd3, idx3[item - 1], 0);
	rdskip(fd3, '/', state+2, 0);
	rdupto(fd3, '/', 1, 0);
#endif
}

/*
	Print a long location description from "advent1.txt"
*/
desclg(loc)
int	loc;
{
#ifdef EMBED
	fputs(adventtxt1[loc-1], stdout);
#else
	fseek(fd1, idx1[loc - 1], 0);
	rdupto(fd1, '#', 1, 0);
#endif
}

/*
	Print a short location description from "advent2.txt"
*/
descsh(loc)
int	loc;
{
#ifdef EMBED
	fputs(adventtxt2[loc-1], stdout);
#else
	fseek(fd2, idx2[loc - 1], 0);
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
vocab(word, val)
char	*word;
int	val;
{
	int	v1, v2;

	if ((v1 = binary(word, wc, MAXWC)) >= 0) {
		v2 = binary(word, wc, MAXWC-1);
		if (v2 < 0)
			v2 = v1;
		if (!val)
			return(wc[v1].acode < wc[v2].acode\
			       ? wc[v1].acode : wc[v2].acode);
		if (val <= wc[v1].acode)
			return(wc[v1].acode % 1000);
		else if (val <= wc[v2].acode)
			return(wc[v2].acode % 1000);
		else
			return(-1);
	}
	else
		return(-1);
}

binary(w, wctable, maxwc)
char	*w;
int	maxwc;
struct	wac	wctable[];
{
	int	lo, mid, hi, check;

	lo = 0;
	hi = maxwc - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		if ((check = strcmp(w, wctable[mid].aword)) < 0)
			hi = mid - 1;
		else if (check > 0)
			lo = mid + 1;
		else
			return(mid);
	}
	return(-1);
}


/*
	Utility Routines
*/

/*
	Routine to test for darkness
*/
dark()
{
	return(!(cond[loc] & LIGHT) &&
		(!prop[LAMP] ||
		!here(LAMP)));
}

/*
	Routine to tell if an item is present.
*/
here(item)
int	item;
{
	return(place[item] == loc || toting(item));
}

/*
	Routine to tell if an item is being carried.
*/
toting(item)
int	item;
{
	return(place[item] == -1);
}

/*
	Routine to tell if a location causes
	a forced move.
*/
forced(atloc)
int	atloc;
{
	return(cond[atloc] == 2);
}

/*
	Routine true x% of the time.
*/
pct(x)
int	x;
{
	return(rand() % 100 < x);
}

/*
	Routine to tell if player is on
	either side of a two sided object.
*/
at(item)
int	item;
{
	return(place[item] == loc || fixed[item] == loc);
}

/*
	Routine to destroy an object
*/
dstroy(obj)
int	obj;
{
	move(obj, 0);
}

/*
	Routine to move an object
*/
move(obj, where)
int	obj, where;
{
	int	from;

	from = (obj<MAXOBJ) ? place[obj] : fixed[obj];
	if (from>0 && from<=300)
		carry(obj, from);
	drop(obj, where);
}

/*
	Juggle an object
	currently a no-op
*/
juggle(loc)
int	loc;
{
}

/*
	Routine to carry an object
*/
carry(obj, where)
int	obj, where;
{
	if (obj<MAXOBJ){
		if (place[obj] == -1)
			return;
		place[obj]=-1;
		++holding;
	}
}

/*
	Routine to drop an object
*/
drop(obj, where)
int	obj, where;
{
	if (obj<MAXOBJ) {
		if (place[obj] == -1)
			--holding;
		place[obj]=where;
	}
	else
		fixed[obj-MAXOBJ]=where;
}

/*
	routine to move an object and return a
	value used to set the negated prop values
	for the repository.
*/
put(obj, where, pval)
int	obj, where, pval;
{
	move(obj, where);
	return((-1)-pval);
}
/*
	Routine to check for presence
	of dwarves..
*/
dcheck()
{
	int	i;

	for (i =1; i < (DWARFMAX-1); ++i)
		if (dloc[i] == loc)
			return(i);
	return(0);
}

/*
	Determine liquid in the bottle
*/
liq()
{
	int	i, j;
	i=prop[BOTTLE];
	j=-1-i;
	return(liq2(i>j ? i : j));
}

/*
	Determine liquid at a location
*/
liqloc(loc)
int	loc;
{
	if (cond[loc]&LIQUID)
		return(liq2(cond[loc]&WATOIL));
	else
		return(liq2(1));
}

/*
	Convert  0 to WATER
		 1 to nothing
		 2 to OIL
*/
liq2(pbottle)
int	pbottle;
{
	return((1-pbottle)*WATER+(pbottle>>1)*(WATER+OIL));
}

/*
	Fatal error routine
*/
bug(n)
int	n;
{
	printf("Fatal error number %d\n", n);
	exit(-1);
}


	

