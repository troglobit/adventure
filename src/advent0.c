
/*	program ADVENT0.C					*\
\*	execution will read the four adventure text files	*\
\*	files; "advent1.txt", "advent2.txt", "advent3.txt" &	*\
\*	"advent4.txt".  it will create the file "advtext.h"	*\
\*	which is an Index Sequential Access Method (ISAM)	*\
\*	header to be #included into "advent.c" before the	*\
\*	header "advdef.h" is #included.				*/

#include <stdio.h> /* drv = 1.1st file 2.def 3.A	*/
#include "advent.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int Ltoa(int n, char *s)
{
	int sz;
	snprintf(s, 10, "%d%n", n, &sz);
	return sz;
}

char *chomp(char *str)
{
	char *ptr;

	ptr = str + strlen(str) - 1;
        while (ptr >= str && *ptr == '\n')
		*ptr-- = 0;

	return str;
}

char *escape(char *str)
{
	char *ptr;

	for (ptr = str; *ptr; ptr++) {
		if (*ptr != '"')
			continue;

		memmove(ptr + 1, ptr, strlen(ptr) + 1);
		*ptr++ = '\\';
	}

	return str;
}

int txtoh(const char *fn)
{
	FILE *fpt, *fph;
	char buf[80];
	int num = 0;
	char *ptr;
	int idx;

	if (!fn || strlen(fn) > sizeof(buf))
		return -1;

	strcpy(buf, fn);
	ptr = strrchr(buf, '.');
	if (!ptr)
		return -1;
	*ptr-- = 0;
	idx = atoi(ptr++);
	strcat(ptr, ".h");

	fpt = fopen(fn, "r");
	if (!fpt)
		return -1;
	fph = fopen(buf, "w");
	if (!fph) {
		fclose(fpt);
		return -1;
	}

	while (fgets(buf, sizeof(buf), fpt)) {
		if (buf[0] == '#')
			num++;
	}
	rewind(fpt);

	fprintf(fph, "const char *adventtxt%d[%d] = {\n", idx, num);
	idx = 0;
	num = 0;
	while (fgets(buf, sizeof(buf), fpt)) {
		chomp(buf);

		if (buf[0] == '#') {
			if (idx != 0 && num == 0)
				fprintf(fph, "\n\tNULL");
			num = 0;
			if (idx++ != 0)
				fprintf(fph, ",\n");
			fprintf(fph, "// %s", &buf[1]);
			continue;
		}

		num++;
		fprintf(fph, "\n\t\"%s\\n\"", escape(buf));
	}
	fprintf(fph, "\n};\n");

	fclose(fph);
	fclose(fpt);

	return 0;
}

int main(void)
{
	FILE *isam, *fd1, *fd2, *fd3, *fd4;
	char itxt[255], lstr[12];
	int cnt, llen;

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
//		printf("%s", itxt);
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd1), lstr);
			if (!llen) {
				printf("Ltoa err in advent1.txt\n");
				exit(-1);
			}
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}
		}
	}
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx2[MAXLOC] = {\n\t");
	while (fgets(itxt, 255, fd2)) {
//		printf("%s", itxt);
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd2), lstr);
			if (!llen) {
				printf("Ltoa err in advent2.txt\n");
				exit(-1);
			}
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}
		}
	}
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx3[MAXOBJ] = {\n\t");
	while (fgets(itxt, 255, fd3)) {
//		printf("%s", itxt);
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd3), lstr);
			if (!llen) {
				printf("Ltoa err in advent3.txt\n");
				exit(-1);
			}
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}
		}
	}
	fprintf(isam, "%s\n\t};\n\n", lstr);

	cnt = -1;
	lstr[0] = '\0';
	fprintf(isam, "long\tidx4[MAXMSG] = {\n\t");
	while (fgets(itxt, 255, fd4)) {
//		printf("%s", itxt);
		if (itxt[0] == '#') {
			if (lstr[0])
				fprintf(isam, "%s,", lstr);
			llen = Ltoa(ftell(fd4), lstr);
			if (!llen) {
				printf("Ltoa err in advent4.txt\n");
				exit(-1);
			}
			if (++cnt == 5) {
				fprintf(isam, "\n\t");
				cnt = 0;
			}
		}
	}
	fprintf(isam, "%s\n\t};\n\n", lstr);

	fclose(isam);
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
	fclose(fd4);

	printf("Datafile processing done!!\n");

	txtoh("advent1.txt");
	txtoh("advent2.txt");
	txtoh("advent3.txt");
	txtoh("advent4.txt");

	printf("Header file processing done!!\n");

	return 0;
}
