
/*	program ADVENT.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h"		*/

#include <err.h>
#include <stdio.h> /* drv = 1.1st file 2.def 3.A	*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "advent.h"  /* #define preprocessor equates	*/
#include "advword.h" /* definition of "word" array	*/
#include "advcave.h" /* definition of "cave" array	*/
#ifndef BUILTIN
#include "advtext.h" /* definition of "text" arrays	*/
#endif
#include "advdef.h"

#define setmem(l, s, c) memset(l, c, s)

int main(int argc, char *argv[])
{
	int rflag; /* user restore request option	*/

	rflag = 0;
	dbugflg = 0;
	while (--argc > 0) {
		++argv;
		if (**argv != '-')
			break;
		switch (tolower(argv[0][1])) {
		case 'r':
			++rflag;
			continue;
		case 'd':
			++dbugflg;
			continue;
		default:
			printf("unknown flag: %c\n", argv[0][1]);
			continue;
		}
	}

	if (dbugflg < 2)
		dbugflg = 0; /* must request three times	*/
	opentxt();
	initplay();
	if (rflag)
		restore();
	else if (yes(65, 1, 0))
		limit = 1000;
	else
		limit = 330;
	saveflg = 0;
	srand(511); /* seed random	*/
	while (!saveflg)
		turn();
	if (saveflg)
		saveadv();
#ifndef BUILTIN
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
	fclose(fd4);
#endif
	return 0;
}

/* ************************************************************	*/

/*
	Initialize integer arrays with sscanf
*/
void scanint(int *pi, char *str)
{
	while (*str) {
		if ((sscanf(str, "%d,", pi++)) < 1)
			bug(41);      /* failed before EOS	*/
		while (*str++ != ',') /* advance str pointer	*/
			;
	}
}

/*
	Initialization of adventure play variables
*/
void initplay(void)
{
	turns = 0;

	/* initialize location status array */
	setmem(cond, (sizeof(int)) * MAXLOC, 0);
	scanint(&cond[1], "5,1,5,5,1,1,5,17,1,1,");
	scanint(&cond[13], "32,0,0,2,0,0,64,2,");
	scanint(&cond[21], "2,2,0,6,0,2,");
	scanint(&cond[31], "2,2,0,0,0,0,0,4,0,2,");
	scanint(&cond[42], "128,128,128,128,136,136,136,128,128,");
	scanint(&cond[51], "128,128,136,128,136,0,8,0,2,");
	scanint(&cond[79], "2,128,128,136,0,0,8,136,128,0,2,2,");
	scanint(&cond[95], "4,0,0,0,0,1,");
	scanint(&cond[113], "4,0,1,1,");
	scanint(&cond[122], "8,8,8,8,8,8,8,8,8,");

	/* initialize object locations */
	setmem(place, (sizeof(int)) * MAXOBJ, 0);
	scanint(&place[1], "3,3,8,10,11,0,14,13,94,96,");
	scanint(&place[11], "19,17,101,103,0,106,0,0,3,3,");
	scanint(&place[23], "109,25,23,111,35,0,97,");
	scanint(&place[31], "119,117,117,0,130,0,126,140,0,96,");
	scanint(&place[50], "18,27,28,29,30,");
	scanint(&place[56], "92,95,97,100,101,0,119,127,130,");

	/* initialize second (fixed) locations */
	setmem(fixed, (sizeof(int)) * MAXOBJ, 0);
	scanint(&fixed[3], "9,0,0,0,15,0,-1,");
	scanint(&fixed[11], "-1,27,-1,0,0,0,-1,");
	scanint(&fixed[23], "-1,-1,67,-1,110,0,-1,-1,");
	scanint(&fixed[31], "121,122,122,0,-1,-1,-1,-1,0,-1,");
	scanint(&fixed[62], "121,-1,");

	/* initialize default verb messages */
	scanint(actmsg, "0,24,29,0,33,0,33,38,38,42,14,");
	scanint(&actmsg[11], "43,110,29,110,73,75,29,13,59,59,");
	scanint(&actmsg[21], "174,109,67,13,147,155,195,146,110,13,13,");

	/* initialize various flags and other variables */
	setmem(visited, (sizeof(int)) * MAXLOC, 0);
	setmem(prop, (sizeof(int)) * MAXOBJ, 0);
	setmem(&prop[50], (sizeof(int)) * (MAXOBJ - 50), 0xff);
	wzdark = closed = closing = holding = detail = 0;
	limit = 100;
	tally = 15;
	tally2 = 0;
	newloc = 1;
	loc = oldloc = oldloc2 = 3;
	knfloc = 0;
	chloc = 114;
	chloc2 = 140;
	/*	dloc[DWARFMAX-1] = chloc				*/
	scanint(dloc, "0,19,27,33,44,64,114,");
	scanint(odloc, "0,0,0,0,0,0,0,");
	dkill = 0;
	scanint(dseen, "0,0,0,0,0,0,0,");
	clock1 = 30;
	clock2 = 50;
	panic = 0;
	bonus = 0;
	numdie = 0;
	daltloc = 18;
	lmwarn = 0;
	foobar = 0;
	dflag = 0;
	gaveup = 0;
	saveflg = 0;
}

/*
	Open advent?.txt files
*/
#ifndef DATADIR
#define DATADIR "/usr/local/lib/games"
#endif

#define ADV1 DATADIR "/advent1.txt"
#define ADV2 DATADIR "/advent2.txt"
#define ADV3 DATADIR "/advent3.txt"
#define ADV4 DATADIR "/advent4.txt"

void opentxt(void)
{
#ifndef BUILTIN
	fd1 = fopen(ADV1, "r");
	if (!fd1) {
		printf("Sorry, I cannot open %s...\n", ADV1);
		exit(-1);
	}
	fd2 = fopen(ADV2, "r");
	if (!fd2) {
		printf("Sorry, I cannot open %s...\n", ADV2);
		exit(-1);
	}
	fd3 = fopen(ADV3, "r");
	if (!fd3) {
		printf("Sorry, I cannot open %s...\n", ADV3);
		exit(-1);
	}
	fd4 = fopen(ADV4, "r");
	if (!fd4) {
		printf("Sorry, I cannot open %s...\n", ADV4);
		exit(-1);
	}
#endif
}

/*
		save adventure game
*/
void saveadv(void)
{
	char nm[64];
	FILE *fp;

	savefile(1, nm, sizeof(nm));

	fp = fopen(nm, "w");
	if (!fp)
		err(1, "Sorry, I cannot save your game to %s", nm);

	fwrite(&turns,      sizeof(int), 1, fp);
	fwrite(&loc,        sizeof(int), 1, fp);
	fwrite(&oldloc,     sizeof(int), 1, fp);
	fwrite(&oldloc2,    sizeof(int), 1, fp);
	fwrite(&newloc,     sizeof(int), 1, fp);      /* location variables */
	fwrite(&cond[0],    sizeof(int), MAXLOC, fp); /* location status	*/
	fwrite(&place[0],   sizeof(int), MAXOBJ, fp); /* object location	*/
	fwrite(&fixed[0],   sizeof(int), MAXOBJ, fp); /* second object loc	*/
	fwrite(&visited[0], sizeof(int), MAXLOC, fp); /* >0 if has been here	*/
	fwrite(&prop[0],    sizeof(int), MAXOBJ, fp); /* status of object	*/
	fwrite(&tally,      sizeof(int), 1, fp);
	fwrite(&tally2,     sizeof(int), 1, fp);      /* item counts		*/
	fwrite(&limit,      sizeof(int), 1, fp);      /* time limit		*/
	fwrite(&lmwarn,     sizeof(int), 1, fp);      /* lamp warning flag	*/
	fwrite(&wzdark,     sizeof(int), 1, fp);
	fwrite(&closing,    sizeof(int), 1, fp);
	fwrite(&closed,     sizeof(int), 1, fp);      /* game state flags	*/
	fwrite(&holding,    sizeof(int), 1, fp);      /* count of held items	*/
	fwrite(&detail,     sizeof(int), 1, fp);      /* LOOK count		*/
	fwrite(&knfloc,     sizeof(int), 1, fp);      /* knife location	*/
	fwrite(&clock1,     sizeof(int), 1, fp);
	fwrite(&clock2,     sizeof(int), 1, fp);
	fwrite(&panic,      sizeof(int), 1, fp);      /* timing variables	*/
	fwrite(&dloc[0],    sizeof(int), DWARFMAX, fp);  /* dwarf locations	*/
	fwrite(&dflag,      sizeof(int), 1, fp);      /* dwarf flag		*/
	fwrite(&dseen[0],   sizeof(int), DWARFMAX, fp); /* dwarf seen flag	*/
	fwrite(&odloc[0],   sizeof(int), DWARFMAX, fp); /* dwarf old locations	*/
	fwrite(&daltloc,    sizeof(int), 1, fp);      /* alternate appearance	*/
	fwrite(&dkill,      sizeof(int), 1, fp);      /* dwarves killed	*/
	fwrite(&chloc,      sizeof(int), 1, fp);
	fwrite(&chloc2,     sizeof(int), 1, fp);      /* chest locations	*/
	fwrite(&bonus,      sizeof(int), 1, fp);      /* to pass to end	*/
	fwrite(&numdie,     sizeof(int), 1, fp);      /* number of deaths	*/
	fwrite(&object1,    sizeof(int), 1, fp);      /* to help intrans.	*/
	fwrite(&gaveup,     sizeof(int), 1, fp);      /* 1 if he quit early	*/
	fwrite(&foobar,     sizeof(int), 1, fp);      /* fie fie foe foo...	*/
	if (fclose(fp) == -1)
		err(1, "Sorry, I cannot seem to close the save game file %s", nm);

	printf("Game saved to %s -- see you later!\n", nm);
}

/*
	restore saved game handler
*/
void restore(void)
{
	char nm[64];
	FILE *fp;

	savefile(0, nm, sizeof(nm));

	fp = fopen(nm, "r");
	if (fp == NULL) {
		warn("Sorry, cannot find any saved game to load from %s", nm);
		return;
	}

	if (fread(&turns,      sizeof(int), 1, fp) != 1 ||
	    fread(&loc,        sizeof(int), 1, fp) != 1 ||
	    fread(&oldloc,     sizeof(int), 1, fp) != 1 ||
	    fread(&oldloc2,    sizeof(int), 1, fp) != 1 ||
	    fread(&newloc,     sizeof(int), 1, fp) != 1 ||	       /* location variables	*/
	    fread(&cond[0],    sizeof(int), MAXLOC, fp) != MAXLOC ||   /* location status	*/
	    fread(&place[0],   sizeof(int), MAXOBJ, fp) != MAXOBJ ||   /* object location	*/
	    fread(&fixed[0],   sizeof(int), MAXOBJ, fp) != MAXOBJ ||   /* second object loc	*/
	    fread(&visited[0], sizeof(int), MAXLOC, fp) != MAXLOC ||   /* >0 if has been here	*/
	    fread(&prop[0],    sizeof(int), MAXOBJ, fp) != MAXOBJ ||   /* status of object	*/
	    fread(&tally,      sizeof(int), 1, fp) != 1 ||
	    fread(&tally2,     sizeof(int), 1, fp) != 1 ||        /* item counts		*/
	    fread(&limit,      sizeof(int), 1, fp) != 1 ||        /* time limit		*/
	    fread(&lmwarn,     sizeof(int), 1, fp) != 1 ||        /* lamp warning flag	*/
	    fread(&wzdark,     sizeof(int), 1, fp) != 1 ||
	    fread(&closing,    sizeof(int), 1, fp) != 1 ||
	    fread(&closed,     sizeof(int), 1, fp) != 1 ||        /* game state flags	*/
	    fread(&holding,    sizeof(int), 1, fp) != 1 ||        /* count of held items	*/
	    fread(&detail,     sizeof(int), 1, fp) != 1 ||        /* LOOK count		*/
	    fread(&knfloc,     sizeof(int), 1, fp) != 1 ||        /* knife location	*/
	    fread(&clock1,     sizeof(int), 1, fp) != 1 ||
	    fread(&clock2,     sizeof(int), 1, fp) != 1 ||
	    fread(&panic,      sizeof(int), 1, fp) != 1 ||	       /* timing variables	*/
	    fread(&dloc[0],    sizeof(int), DWARFMAX, fp) != DWARFMAX || /* dwarf locations	*/
	    fread(&dflag,      sizeof(int), 1, fp) != 1 ||	       /* dwarf flag		*/
	    fread(&dseen[0],   sizeof(int), DWARFMAX, fp) != DWARFMAX || /* dwarf seen flag	*/
	    fread(&odloc[0],   sizeof(int), DWARFMAX, fp) != DWARFMAX || /* dwarf old locations	*/
	    fread(&daltloc,    sizeof(int), 1, fp) != 1 ||	       /* alternate appearance	*/
	    fread(&dkill,      sizeof(int), 1, fp) != 1 ||	       /* dwarves killed	*/
	    fread(&chloc,      sizeof(int), 1, fp) != 1 ||
	    fread(&chloc2,     sizeof(int), 1, fp) != 1 ||        /* chest locations	*/
	    fread(&bonus,      sizeof(int), 1, fp) != 1 ||        /* to pass to end	*/
	    fread(&numdie,     sizeof(int), 1, fp) != 1 ||        /* number of deaths	*/
	    fread(&object1,    sizeof(int), 1, fp) != 1 ||        /* to help intrans.	*/
	    fread(&gaveup,     sizeof(int), 1, fp) != 1 ||        /* 1 if he quit early	*/
	    fread(&foobar,     sizeof(int), 1, fp) != 1) {        /* fie fie foe foo...	*/
		warn("Failed reading saved gave file, %s, data format error", nm);
		fclose(fp);
		return;
	}

	fclose(fp);

	printf("Game restored from %s\n", nm);
	describe();
}

char *savefile(int save, char *path, size_t len)
{
	char *home;

#ifndef SAVEDIR
	(void)save;

	home = getenv("HOME");
	if (!home)
		home = ".";
	snprintf(path, len, "%s/.adventure", home);
#else
	char fn[32];

	if (save)
		printf("What do you want to name the saved game? ");
	else
		printf("What is the name of the saved game? ");
	fflush(stdout);

	if (!fgets(fn, 32, stdin)) {
		home = getenv("LOGNAME");
		if (!home)
			exit(0);
		snprintf(fn, sizeof(fn), "%s", home);
	} else {
		char *sptr;

		if ((sptr = strchr(fn, '\n')))
			*sptr = '\0';
		if ((sptr = strchr(fn, '\r')))
			*sptr = '\0';
		if ((sptr = strchr(fn, '.')))
			*sptr = '\0'; /* kill extension	*/
	}

	if (strlen(fn) > 8)
		fn[8] = '\0'; /* max 8 char filename	*/
	strcat(fn, ".adv");

	snprintf(path, len, "%s/%s", SAVEDIR, fn);

#endif

	return path;
}
