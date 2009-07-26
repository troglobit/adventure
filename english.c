
/*	program ENGLISH.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"
#include	"advdec.h"

#ifndef __QNX__
extern	char	*fgets();
extern	int	fputc();
extern	int	printf();
extern	int	sscanf();
#else
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define atoi atol
#define rindex strchr
#endif



/*
	Analyze a two word sentence
*/
english()
{

	char	*msg;
	int	type1, type2, val1, val2;

	verb = object = motion = 0;
	type2 = val2 = -1;
	type1 = val1 = -1;
	msg = "bad grammar...";

	getwords();

	if (!(*word1))
		return(0);		/* ignore whitespace	*/
	if (!analyze(word1, &type1, &val1))	/* check word1	*/
		return(0);		/* didn't know it	*/

	if (type1 == 2 && val1 == SAY) {
		verb = SAY;	/* repeat word & act upon if..	*/
		object = 1;
		return(1);
	}

	if (*word2)
		if (!analyze(word2, &type2, &val2))
			return(0);	/* didn't know it	*/

	/* check his grammar */
	if ((type1 == 3) && (type2 == 3) && \
	    (val1 == 51) && (val2 == 51)) {
		outwords();
		return(0);
	}
	else if (type1 == 3) {
		rspeak(val1);
		return(0);
	}
	else if (type2 == 3) {
		rspeak(val2);
		return(0);
	}
	else if (type1 == 0) {
		if (type2 == 0) {
			printf("%s\n", msg);
			return(0);
		}
		else
			motion = val1;
	}
	else if (type2 == 0)
		motion = val2;
	else if (type1 == 1) {
		object = val1;
		if (type2 == 2)
			verb = val2;
		if (type2 == 1) {
			printf("%s\n", msg);
			return(0);
		}
	}
	else if (type1 == 2) {
		verb = val1;
		if (type2 == 1)
			object = val2;
		if (type2 == 2) {
			printf("%s\n", msg);
			return(0);
		}
	}
	else
		bug(36);
	return(1);
}


/*
		Routine to analyze a word.
*/
analyze(word, type, value)
char	*word;
int	*type, *value;
{
	int	wordval, msg;

	/* make sure I understand */
	if ((wordval = vocab(word, 0)) == -1) {
		switch(rand() % 3) {
		case 0:
			msg = 60;
			break;
		case 1:
			msg = 61;
			break;
		default:
			msg = 13;
		}
		rspeak(msg);
		return(0);
	}
	*type = wordval/1000;
	*value = wordval%1000;
	return(1);
}

/*
	retrieve input line (max 80 chars), convert to lower case
	 & rescan for first two words (max. WORDSIZE-1 chars).
*/
getwords()
{
	char	words[80], *wptr;

	fputc('>', stdout);
	word1[0] = word2[0] = '\0';
	fflush(stdout);
	if (NULL == fgets(words, 80, stdin))
		exit(0);
	wptr = words;
	while (*wptr = tolower(*wptr))
		++wptr;
	sscanf(words, "%19s %19s", word1, word2);
	if (dbugflg)
		printf("WORD1 = %s, WORD2 = %s\n", word1, word2);
	return;
}

/*
	output adventure word list (motion/0xxx & verb/2xxx) only
	6 words/line pausing at 20th line until keyboard active
*/
outwords()
{
	int	i, j, line;
	char	words[80];

	j = line = 0;
	for (i = 0; i < MAXWC; ++i) {
		if ((wc[i].acode < 1000) || ((wc[i].acode < 3000) && \
		                             (wc[i].acode > 1999))) {
			printf("%-12s", wc[i].aword);
			if ((++j == 6) || (i == MAXWC-1)) {
				j = 0;
				fputc('\n', stdout);
				if (++line == 20) {
					line = 0;
					printf("\n\007Enter <RETURN>");
					printf(" to continue\n\n");
					fflush(stdout);
					if (NULL == fgets(words, 80, stdin))
						exit(0);
				}
			}
		}
	}
}


