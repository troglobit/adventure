
/*	header ADVDEF.H						*\
\*	WARNING: GLOBAL variable allocations for adventure	*/

/*
	Database variables
*/
struct trav travel[MAXTRAV];
#ifndef BUILTIN
FILE *fd1, *fd2, *fd3, *fd4;
#endif
int actmsg[32]; /* action messages	*/

/*
	English variables
*/
int verb, object, motion;
char word1[WORDSIZE], word2[WORDSIZE];

/*
	Play variables
*/
int turns;
int loc, oldloc, oldloc2, newloc; /* location variables */
int cond[MAXLOC];		  /* location status	*/
int place[MAXOBJ];		  /* object location	*/
int fixed[MAXOBJ];		  /* second object loc	*/
int visited[MAXLOC];		  /* >0 if has been here	*/
int prop[MAXOBJ];		  /* status of object	*/
int tally, tally2;		  /* item counts		*/
int limit;			  /* time limit		*/
int lmwarn;			  /* lamp warning flag	*/
int wzdark, closing, closed;      /* game state flags	*/
int holding;			  /* count of held items	*/
int detail;			  /* LOOK count		*/
int knfloc;			  /* knife location	*/
int clock1, clock2, panic;	  /* timing variables	*/
int dloc[DWARFMAX];		  /* dwarf locations	*/
int dflag;			  /* dwarf flag		*/
int dseen[DWARFMAX];		  /* dwarf seen flag	*/
int odloc[DWARFMAX];		  /* dwarf old locations	*/
int daltloc;			  /* alternate appearance	*/
int dkill;			  /* dwarves killed	*/
int chloc, chloc2;		  /* chest locations	*/
int bonus;			  /* to pass to end	*/
int numdie;			  /* number of deaths	*/
int object1;			  /* to help intrans.	*/
int gaveup;			  /* 1 if he quit early	*/
int foobar;			  /* fie fie foe foo...	*/
int saveflg;			  /* if game being saved	*/
int dbugflg;			  /* if game is in debug	*/
int lastglob;			  /* to get space req.	*/
