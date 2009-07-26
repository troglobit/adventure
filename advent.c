
/*	program ADVENT.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h"		*/


#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"	/* #define preprocessor equates	*/
#include	"advword.h"	/* definition of "word" array	*/
#include	"advcave.h"	/* definition of "cave" array	*/
#ifndef EMBED
#include	"advtext.h"	/* definition of "text" arrays	*/
#endif
#include	"advdef.h"

#ifndef __QNX__
#define	strchr	index

extern	int	fclose();
extern	int	fgetc();
extern	FILE	*fopen();
extern	int	fputc();
extern	long	ftell();
extern	int	printf();
extern	int	setmem();
extern	int	scanf();
extern	int	sscanf();
extern	char	*strcat();
extern	char	*strchr();
extern	unsigned	strlen();
extern	int	tolower();
#else
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define setmem(l,s,c)  memset(l,c,s)
#endif

main(argc, argv)
int	argc;
char	**argv;
{
	int	rflag;		/* user restore request option	*/

	rflag = 0;
	dbugflg = 0;
	while (--argc > 0) {
		++argv;
		if (**argv !=  '-')
			break;
		switch(tolower(argv[0][1])) {
		case 'r':
			++rflag;
			continue;
		case 'd':
			++dbugflg;
			continue;
		default:
			printf("unknown flag: %c\n", argv[0][1]);
			continue;
		}				/* switch	*/
	}					/* while	*/
	if (dbugflg < 2)
		dbugflg = 0;	/* must request three times	*/
	opentxt();
	initplay();
	if (rflag)
		restore();
	else if (yes(65, 1, 0))
		limit = 1000;
	else
		limit = 330;
	saveflg = 0;
	srand(511);				/* seed random	*/
	while(!saveflg)
		turn();
	if (saveflg)
		saveadv();
#ifndef EMBED
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
	fclose(fd4);
#endif
	exit(0);				/* exit = ok	*/
}						/* main		*/

/* ************************************************************	*/

/*
	Initialize integer arrays with sscanf
*/
scanint(pi, str)
int	*pi;
char	*str;
{

	while (*str) {
		if  ((sscanf(str, "%d,", pi++)) < 1)
			bug(41);	/* failed before EOS	*/
		while (*str++ != ',')	/* advance str pointer	*/
			;
	}
	return;
}

/*
	Initialization of adventure play variables
*/
initplay()
{
	turns = 0;

	/* initialize location status array */
	setmem(cond, (sizeof(int))*MAXLOC, 0); 
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
	setmem(place, (sizeof(int))*MAXOBJ, 0); 
	scanint(&place[1], "3,3,8,10,11,0,14,13,94,96,");
	scanint(&place[11], "19,17,101,103,0,106,0,0,3,3,");
	scanint(&place[23], "109,25,23,111,35,0,97,");
	scanint(&place[31], "119,117,117,0,130,0,126,140,0,96,");
	scanint(&place[50], "18,27,28,29,30,");
	scanint(&place[56], "92,95,97,100,101,0,119,127,130,");

	/* initialize second (fixed) locations */
	setmem(fixed, (sizeof(int))*MAXOBJ, 0); 
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
	setmem(visited, (sizeof(int))*MAXLOC, 0);
	setmem(prop, (sizeof(int))*MAXOBJ, 0);
	setmem(&prop[50], (sizeof(int))*(MAXOBJ-50), 0xff); 
	wzdark = closed = closing = holding = detail = 0;
	limit = 100;
	tally = 15;
	tally2 = 0;
	newloc = 3;
	loc = oldloc = oldloc2 = 1;
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
	return;
}

/*
	Open advent?.txt files
*/

#define ADV1 "/usr/local/lib/games/advent1.txt"
#define ADV2 "/usr/local/lib/games/advent2.txt"
#define ADV3 "/usr/local/lib/games/advent3.txt"
#define ADV4 "/usr/local/lib/games/advent4.txt"

opentxt()
{
#ifndef EMBED
	fd1 = fopen(ADV1, "r");
	if (!fd1) {
		printf("Sorry, I can't open advent1.txt...\n");
		exit(-1);
	}
	fd2 = fopen(ADV2, "r");
	if (!fd2) {
		printf("Sorry, I can't open advent2.txt...\n");
		exit(-1);
	}
	fd3 = fopen(ADV3, "r");
	if (!fd3) {
		printf("Sorry, I can't open advent3.txt...\n");
		exit(-1);
	}
	fd4 = fopen(ADV4, "r");
	if (!fd4) {
		printf("Sorry, I can't open advent4.txt...\n");
		exit(-1);
	}
#endif
}

/*
		save adventure game
*/
saveadv()
{
	char	*sptr;
	FILE	*savefd;
	char	username[64];

#ifndef __QNX__
	printf("What do you want to name the saved game? ");
	fflush(stdout);
	if (NULL == gets(username))
		exit(0);
	if (sptr = strchr(username, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(username) > 8)
		username[8] = '\0';	/* max 8 char filename	*/
	strcat(username, ".adv");
#else
	game_name(username);
#endif
	savefd = fopen(username, "w");
	if (savefd == NULL) {
		printf("Sorry, I can't save your game...\nAborting program...\n");
		exit(-1);
	}
	fwrite( &turns, sizeof(int), 1, savefd);
	fwrite( &loc, sizeof(int), 1, savefd);
	fwrite( &oldloc, sizeof(int), 1, savefd);
	fwrite( &oldloc2, sizeof(int), 1, savefd);
	fwrite( &newloc, sizeof(int), 1, savefd);	/* location variables */
	fwrite( &cond[0], sizeof(int), MAXLOC, savefd);			/* location status	*/
	fwrite( &place[0], sizeof(int), MAXOBJ, savefd);		/* object location	*/
	fwrite( &fixed[0], sizeof(int), MAXOBJ, savefd);		/* second object loc	*/
	fwrite( &visited[0], sizeof(int), MAXLOC, savefd);		/* >0 if has been here	*/
	fwrite( &prop[0], sizeof(int), MAXOBJ, savefd);			/* status of object	*/
	fwrite( &tally, sizeof(int), 1, savefd);
	fwrite( &tally2, sizeof(int), 1, savefd);		/* item counts		*/
	fwrite( &limit, sizeof(int), 1, savefd);			/* time limit		*/
	fwrite( &lmwarn, sizeof(int), 1, savefd);			/* lamp warning flag	*/
	fwrite( &wzdark, sizeof(int), 1, savefd);
	fwrite( &closing, sizeof(int), 1, savefd);
	fwrite( &closed, sizeof(int), 1, savefd);	/* game state flags	*/
	fwrite( &holding, sizeof(int), 1, savefd);		/* count of held items	*/
	fwrite( &detail, sizeof(int), 1, savefd);			/* LOOK count		*/
	fwrite( &knfloc, sizeof(int), 1, savefd);			/* knife location	*/
	fwrite( &clock1, sizeof(int), 1, savefd);
	fwrite( &clock2, sizeof(int), 1, savefd);
	fwrite( &panic, sizeof(int), 1, savefd);	/* timing variables	*/
	fwrite( &dloc[0], sizeof(int), DWARFMAX, savefd);			/* dwarf locations	*/
	fwrite( &dflag, sizeof(int), 1, savefd);			/* dwarf flag		*/
	fwrite( &dseen[0], sizeof(int), DWARFMAX, savefd);		/* dwarf seen flag	*/
	fwrite( &odloc[0], sizeof(int), DWARFMAX, savefd);		/* dwarf old locations	*/
	fwrite( &daltloc, sizeof(int), 1, savefd);		/* alternate appearance	*/
	fwrite( &dkill, sizeof(int), 1, savefd);			/* dwarves killed	*/
	fwrite( &chloc, sizeof(int), 1, savefd);
	fwrite( &chloc2, sizeof(int), 1, savefd);		/* chest locations	*/
	fwrite( &bonus, sizeof(int), 1, savefd);			/* to pass to end	*/
	fwrite( &numdie, sizeof(int), 1, savefd);			/* number of deaths	*/
	fwrite( &object1, sizeof(int), 1, savefd);		/* to help intrans.	*/
	fwrite( &gaveup, sizeof(int), 1, savefd);			/* 1 if he quit early	*/
	fwrite( &foobar, sizeof(int), 1, savefd);			/* fie fie foe foo...	*/
	if (fclose(savefd)  ==  -1) {
		printf("Sorry, I can't close the file...%s\n", \
		       username);
		exit(-1);
	}
	printf("Game saved -- see you later...\n");
}

/*
	restore saved game handler
*/
restore()
{
	char	username[64];
	FILE *restfd;
	int c;
	char	*sptr;

#ifndef __QNX__
	printf("What is the name of the saved game? ");
	fflush(stdout);
	if (NULL == gets(username))
		exit(0);
	if (sptr = strchr(username, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(username) > 8)
		username[8] = '\0';	/* max 8 char filename	*/
	strcat(username, ".adv"); 
#else
	game_name(username);
#endif
	restfd = fopen(username, "r");
	if (restfd == NULL) {
		printf("Sorry, no game to load...\n");
		return;
	}
	fread( &turns, sizeof(int), 1, restfd);
	fread( &loc, sizeof(int), 1, restfd);
	fread( &oldloc, sizeof(int), 1, restfd);
	fread( &oldloc2, sizeof(int), 1, restfd);
	fread( &newloc, sizeof(int), 1, restfd);	/* location variables */
	fread( &cond[0], sizeof(int), MAXLOC, restfd);			/* location status	*/
	fread( &place[0], sizeof(int), MAXOBJ, restfd);		/* object location	*/
	fread( &fixed[0], sizeof(int), MAXOBJ, restfd);		/* second object loc	*/
	fread( &visited[0], sizeof(int), MAXLOC, restfd);		/* >0 if has been here	*/
	fread( &prop[0], sizeof(int), MAXOBJ, restfd);			/* status of object	*/
	fread( &tally, sizeof(int), 1, restfd);
	fread( &tally2, sizeof(int), 1, restfd);		/* item counts		*/
	fread( &limit, sizeof(int), 1, restfd);			/* time limit		*/
	fread( &lmwarn, sizeof(int), 1, restfd);			/* lamp warning flag	*/
	fread( &wzdark, sizeof(int), 1, restfd);
	fread( &closing, sizeof(int), 1, restfd);
	fread( &closed, sizeof(int), 1, restfd);	/* game state flags	*/
	fread( &holding, sizeof(int), 1, restfd);		/* count of held items	*/
	fread( &detail, sizeof(int), 1, restfd);			/* LOOK count		*/
	fread( &knfloc, sizeof(int), 1, restfd);			/* knife location	*/
	fread( &clock1, sizeof(int), 1, restfd);
	fread( &clock2, sizeof(int), 1, restfd);
	fread( &panic, sizeof(int), 1, restfd);	/* timing variables	*/
	fread( &dloc[0], sizeof(int), DWARFMAX, restfd);			/* dwarf locations	*/
	fread( &dflag, sizeof(int), 1, restfd);			/* dwarf flag		*/
	fread( &dseen[0], sizeof(int), DWARFMAX, restfd);		/* dwarf seen flag	*/
	fread( &odloc[0], sizeof(int), DWARFMAX, restfd);		/* dwarf old locations	*/
	fread( &daltloc, sizeof(int), 1, restfd);		/* alternate appearance	*/
	fread( &dkill, sizeof(int), 1, restfd);			/* dwarves killed	*/
	fread( &chloc, sizeof(int), 1, restfd);
	fread( &chloc2, sizeof(int), 1, restfd);		/* chest locations	*/
	fread( &bonus, sizeof(int), 1, restfd);			/* to pass to end	*/
	fread( &numdie, sizeof(int), 1, restfd);			/* number of deaths	*/
	fread( &object1, sizeof(int), 1, restfd);		/* to help intrans.	*/
	fread( &gaveup, sizeof(int), 1, restfd);			/* 1 if he quit early	*/
	fread( &foobar, sizeof(int), 1, restfd);			/* fie fie foe foo...	*/
	if (fclose(restfd)  ==  -1) {
		printf("Warning -- can't close save file...%s\n", \
		       username);
	}
	printf("Game restored...\n");
	describe();
}

char *game_name(filename)
char *filename;
{
#ifndef EMBED
	char *homedir;

	filename[0] = 0;
	if(homedir = getenv("HOME")) {
		strcat(filename, homedir);
		strcat(filename, "/");
	}
	strcat(filename, ".adventure");
	return(filename);
#else
	char  username[32];
	char *sptr;
	
	printf("What is the name of the saved game? ");
	fflush(stdout);
	if (NULL == fgets(username, 32, stdin))
		exit(0);
	if (sptr = strchr(username, '\n'))
		*sptr = '\0';
	if (sptr = strchr(username, '\r'))
		*sptr = '\0';
	if (sptr = strchr(username, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(username) > 8)
		username[8] = '\0';	/* max 8 char filename	*/
	strcat(username, ".adv");
	strcpy(filename, "/var/tmp/");
	strcat(filename, username);
#endif
}


