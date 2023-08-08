
/*	header ADVDEC.H						*\
\*	WARNING: GLOBAL EXTERNAL declarations for adventure	*/

/*
	Database variables
*/
extern struct wac wc[]; /* see ADVWORD.H		*/
extern char *cave[];    /* see ADVCAVE.H		*/
extern size_t cavesz;
#ifndef BUILTIN
extern long idx1[]; /* see ADVTEXT.H		*/
extern long idx2[]; /* see ADVTEXT.H		*/
extern long idx3[]; /* see ADVTEXT.H		*/
extern long idx4[]; /* see ADVTEXT.H		*/
#endif

extern struct trav travel[];
#ifndef BUILTIN
extern FILE *fd1, *fd2, *fd3, *fd4;
#endif
extern int actmsg[]; /* action messages	*/

/*
	English variables
*/
extern int verb, object, motion;
extern char word1[], word2[];

/*
	Play variables
*/
extern int turns;
extern int loc, oldloc, oldloc2, newloc; /* location variables  */
extern int cond[];			 /* location status	*/
extern int place[];			 /* object location	*/
extern int fixed[];			 /* second object loc	*/
extern int visited[];			 /* >0 if has been here	*/
extern int prop[];			 /* status of object	*/
extern int tally, tally2;		 /* item counts		*/
extern int limit;			 /* time limit		*/
extern int lmwarn;			 /* lamp warning flag	*/
extern int wzdark, closing, closed;      /* game state flags	*/
extern int holding;			 /* count of held items	*/
extern int detail;			 /* LOOK count		*/
extern int knfloc;			 /* knife location	*/
extern int clock1, clock2, panic;	 /* timing variables	*/
extern int dloc[];			 /* dwarf locations	*/
extern int dflag;			 /* dwarf flag		*/
extern int dseen[];			 /* dwarf seen flag	*/
extern int odloc[];			 /* dwarf old locations	*/
extern int daltloc;			 /* alternate appearance*/
extern int dkill;			 /* dwarves killed	*/
extern int chloc, chloc2;		 /* chest locations	*/
extern int bonus;			 /* to pass to end	*/
extern int numdie;			 /* number of deaths	*/
extern int object1;			 /* to help intrans.	*/
extern int gaveup;			 /* 1 if he quit early	*/
extern int foobar;			 /* fie fie foe foo...	*/
extern int saveflg;			 /* if game being saved	*/
extern int dbugflg;			 /* if game is in debug	*/
extern int lastglob;			 /* to get space req.	*/
