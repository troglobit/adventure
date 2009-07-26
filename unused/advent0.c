
/*	program ADVENT0.C					*\
\*	execution will read the four adventure text files	*\
\*	files; "advent1.txt", "advent2.txt", "advent3.txt" &	*\
\*	"advent4.txt".  it will create the file "advtext.h"	*\
\*	which is an Index Sequential Access Method (ISAM)	*\
\*	header to be #included into "advent.c" before the	*\
\*	header "advdef.h" is #included.				*/


#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"

#include <stdlib.h>
#include <ctype.h>

int Ltoa(int n, char *s) {
int sz;
	snprintf(s, 10, "%d%n", n, &sz);
	return sz;
}

int
main(argc, argv)
int	argc;
char	**argv;
{

	FILE	*isam, *fd1, *fd2, *fd3, *fd4;
	char	itxt[255], otxt[80], lstr[12];
	int	cnt, llen;

	isam = fopen("advtext.h", "w");
	if (!isam) {
		printf("Sorry, I can't open advtext.h...\n");
		exit(-1);
	}
	fd1 = fopen("advent1.txt", "r");
	if (!fd1) {
		printf("Sorry, I can't open advent1.txt...\n");
		exit(-1);
	}
	fd2 = fopen("advent2.txt", "r");
	if (!fd2) {
		printf("Sorry, I can't open advent2.txt...\n");
		exit(-1);
	}
	fd3 = fopen("advent3.txt", "r");
	if (!fd3) {
		printf("Sorry, I can't open advent3.txt...\n");
		exit(-1);
	}
	fd4 = fopen("advent4.txt", "r");
	if (!fd4) {
		printf("Sorry, I can't open advent4.txt...\n");
		exit(-1);
	}

	fprintf(isam, "\n/");
	fprintf(isam, "*\theader: ADVTEXT.H\t\t\t\t\t*/\n\n\n");


	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx1[MAXLOC] = {\n\t");
	while (fgets(itxt, 255, fd1)) {
/*		printf("%s", itxt); */
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd1), lstr);
			if (!llen) {
				printf("Ltoa err in advent1.txt\n");
				exit(-1);
			}			/* if (!llen)	*/
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}			/* if (cnt)	*/
		}				/* if (itxt[0])	*/
	}					/* while fgets	*/
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx2[MAXLOC] = {\n\t");
	while (fgets(itxt, 255, fd2)) {
/*		printf("%s", itxt); */
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd2), lstr);
			if (!llen) {
				printf("Ltoa err in advent2.txt\n");
				exit(-1);
			}			/* if (!llen)	*/
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}			/* if (cnt)	*/
		}				/* if (itxt[0])	*/
	}					/* while fgets	*/
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx3[MAXOBJ] = {\n\t");
	while (fgets(itxt, 255, fd3)) {
/*		printf("%s", itxt); */
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd3), lstr);
			if (!llen) {
				printf("Ltoa err in advent3.txt\n");
				exit(-1);
			}			/* if (!llen)	*/
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}			/* if (cnt)	*/
		}				/* if (itxt[0])	*/
	}					/* while fgets	*/
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx4[MAXMSG] = {\n\t");
	while (fgets(itxt, 255, fd4)) {
/*		printf("%s", itxt); */
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd4), lstr);
			if (!llen) {
				printf("Ltoa err in advent4.txt\n");
				exit(-1);
			}			/* if (!llen)	*/
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}			/* if (cnt)	*/
		}				/* if (itxt[0])	*/
	}					/* while fgets	*/
	fprintf(isam, "%s\n\t};\n\n", lstr);

	fclose(isam);
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
	fclose(fd4);
	printf("Datafile processing done!!\n");
	return(0);
}						/* main		*/


