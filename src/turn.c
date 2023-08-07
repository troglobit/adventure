
/*	program TURN.C						*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/

#include <stdio.h> /* drv = 1.1st file 2.def 3.A	*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "advent.h"
#include "advdec.h"


/*
	Routine to take 1 turn
*/
void turn(void)
{
	int i;
	/*
		if closing, then he can't leave except via
		the main office.
	*/
	if (newloc < 9 && newloc != 0 && closing) {
		rspeak(130);
		newloc = loc;
		if (!panic)
			clock2 = 15;
		panic = 1;
	}
	/*
		see if a dwarf has seen him and has come
		from where he wants to go.
	*/
	if (newloc != loc && !forced(loc) && (cond[loc] & NOPIRAT) == 0) {
		for (i = 1; i < (DWARFMAX - 1); ++i) {
			if (odloc[i] == newloc && dseen[i]) {
				newloc = loc;
				rspeak(2);
				break;
			}
		}
	}

	dwarves(); /* & special dwarf(pirate who steals)	*/

	/* added by BDS C conversion */
	if (loc != newloc) {
		++turns;
		loc = newloc;
/*	causes occasional "move" with two describe & descitem	*/
/*	}	*/
/*	if (loc != newloc)	*/

		/* check for death */
		if (loc == 0) {
			death();
			return;
		}

		/* check for forced move */
		if (forced(loc)) {
			describe();
			domove();
			return;
		}

		/* check for wandering in dark */
		if (wzdark && dark() && pct(35)) {
			rspeak(23);
			oldloc2 = loc;
			death();
			return;
		}

		/* describe his situation */
		describe();
		if (!dark()) {
			++visited[loc];
			descitem();
		}
/*	causes occasional "move" with no describe & descitem	*/
	}

	if (closed) {
		if (prop[OYSTER] < 0 && toting(OYSTER))
			pspeak(OYSTER, 1);
		for (i = 1; i < MAXOBJ; ++i) {
			if (toting(i) && prop[i] < 0)
				prop[i] = -1 - prop[i];
		}
	}

	wzdark = dark();
	if (knfloc > 0 && knfloc != loc)
		knfloc = 0;

	if (stimer()) /* as the grains of sand slip by	*/
		return;

	while (!english()) /* retrieve player instructions	*/
		;

	if (dbugflg)
		printf("loc = %d, verb = %d, object = %d, \
		motion = %d\n",
		       loc, verb, object, motion);

	if (motion) /* execute player instructions	*/
		domove();
	else if (object)
		doobj();
	else
		itverb();
}

/*
	Routine to describe current location
*/
void describe(void)
{
	if (toting(BEAR))
		rspeak(141);
	if (dark())
		rspeak(16);
	else if (visited[loc])
		descsh(loc);
	else
		desclg(loc);
	if (loc == 33 && pct(25) && !closing)
		rspeak(8);
}

/*
	Routine to describe visible items
*/
void descitem(void)
{
	int i, state;

	for (i = 1; i < MAXOBJ; ++i) {
		if (at(i)) {
			if (i == STEPS && toting(NUGGET))
				continue;
			if (prop[i] < 0) {
				if (closed)
					continue;

				prop[i] = 0;
				if (i == RUG || i == CHAIN)
					++prop[i];
				--tally;
			}
			if (i == STEPS && loc == fixed[STEPS])
				state = 1;
			else
				state = prop[i];
			pspeak(i, state);
		}
	}
	if (tally == tally2 && tally != 0 && limit > 35)
		limit = 35;
}

/*
	Routine to handle motion requests
*/
void domove(void)
{
	gettrav(loc);
	switch (motion) {
	case NULLX:
		break;
	case BACK:
		goback();
		break;
	case LOOK:
		if (detail++ < 3)
			rspeak(15);
		wzdark = 0;
		visited[loc] = 0;
		newloc = loc;
		loc = 0;
		break;
	case CAVE:
		if (loc < 8)
			rspeak(57);
		else
			rspeak(58);
		break;
	default:
		oldloc2 = oldloc;
		oldloc = loc;
		dotrav();
	}
}

/*
	Routine to handle request to return
	from whence we came!
*/
void goback(void)
{
	int kk, k2, want, temp;
	struct trav strav[MAXTRAV];

	if (forced(oldloc))
		want = oldloc2;
	else
		want = oldloc;
	oldloc2 = oldloc;
	oldloc = loc;
	k2 = 0;
	if (want == loc) {
		rspeak(91);
		return;
	}
	copytrv(travel, strav);
	for (kk = 0; travel[kk].tdest != -1; ++kk) {
		if (!travel[kk].tcond && travel[kk].tdest == want) {
			motion = travel[kk].tverb;
			dotrav();
			return;
		}
		if (!travel[kk].tcond) {
			k2 = kk;
			temp = travel[kk].tdest;
			gettrav(temp);
			if (forced(temp) && travel[0].tdest == want)
				k2 = temp;
			copytrv(strav, travel);
		}
	}
	if (k2) {
		motion = travel[k2].tverb;
		dotrav();
	} else
		rspeak(140);
}

/*
	Routine to copy a travel array
*/
void copytrv(struct trav *trav1, struct trav *trav2)
{
	int i;

	for (i = 0; i < MAXTRAV; ++i) {
		trav2->tdest = trav1->tdest;
		trav2->tverb = trav1->tverb;
		trav2->tcond = trav1->tcond;
	}
}

/*
	Routine to figure out a new location
	given current location and a motion.
*/
void dotrav(void)
{
	char mvflag, hitflag;
	int rdest, rverb, rcond, robject;
	int pctt, kk;

	newloc = loc;
	mvflag = hitflag = 0;
	pctt = rand() % 100;

	for (kk = 0; travel[kk].tdest >= 0 && !mvflag; ++kk) {
		rdest = travel[kk].tdest;
		rverb = travel[kk].tverb;
		rcond = travel[kk].tcond;
		robject = rcond % 100;

		if (dbugflg)
			printf("rdest = %d, rverb = %d, rcond = %d, \
			robject = %d in dotrav\n",
			       rdest, rverb, rcond, robject);
		if ((rverb != 1) && (rverb != motion) && !hitflag)
			continue;
		++hitflag;
		switch (rcond / 100) {
		case 0:
			if ((rcond == 0) || (pctt < rcond))
				++mvflag;
			if (rcond && dbugflg)
				printf("%% move %d %d\n", pctt, mvflag);
			break;
		case 1:
			if (robject == 0)
				++mvflag;
			else if (toting(robject))
				++mvflag;
			break;
		case 2:
			if (toting(robject) || at(robject))
				++mvflag;
			break;
		case 3:
		case 4:
		case 5:
		case 7:
			if (prop[robject] != (rcond / 100) - 3)
				++mvflag;
			break;
		default:
			bug(37);
		}
	}
	if (!mvflag)
		badmove();
	else if (rdest > 500)
		rspeak(rdest - 500);
	else if (rdest > 300)
		spcmove(rdest);
	else {
		newloc = rdest;
		if (dbugflg)
			printf("newloc in dotrav = %d\n", newloc);
	}
}

/*
	The player tried a poor move option.
*/
void badmove(void)
{
	int msg;

	msg = 12;
	if (motion >= 43 && motion <= 50)
		msg = 9;
	if (motion == 29 || motion == 30)
		msg = 9;
	if (motion == 7 || motion == 36 || motion == 37)
		msg = 10;
	if (motion == 11 || motion == 19)
		msg = 11;
	if (verb == FIND || verb == INVENTORY)
		msg = 59;
	if (motion == 62 || motion == 65)
		msg = 42;
	if (motion == 17)
		msg = 80;
	rspeak(msg);
}

/*
	Routine to handle very special movement.
*/
void spcmove(int rdest)
{
	switch (rdest - 300) {
	case 1: /* plover movement via alcove */
		if (!holding || (holding == 1 && toting(EMERALD)))
			newloc = (99 + 100) - loc;
		else
			rspeak(117);
		break;
	case 2: /* trying to remove plover, bad route */
		drop(EMERALD, loc);
		break;
	case 3: /* troll bridge */
		if (prop[TROLL] == 1) {
			pspeak(TROLL, 1);
			prop[TROLL] = 0;
			move(TROLL2, 0);
			move((TROLL2 + MAXOBJ), 0);
			move(TROLL, 117);
			move((TROLL + MAXOBJ), 122);
			juggle(CHASM);
			newloc = loc;
		} else {
			newloc = (loc == 117 ? 122 : 117);
			if (prop[TROLL] == 0)
				++prop[TROLL];
			if (!toting(BEAR))
				return;
			rspeak(162);
			prop[CHASM] = 1;
			prop[TROLL] = 2;
			drop(BEAR, newloc);
			fixed[BEAR] = -1;
			prop[BEAR] = 3;
			if (prop[SPICES] < 0)
				++tally2;
			oldloc2 = newloc;
			death();
		}
		break;
	default:
		bug(38);
	}
}

/*
	Routine to handle player's demise via
	waking up the dwarves...
*/
void dwarfend(void)
{
	death();
	normend();
}

/*
	normal end of game
*/
void normend(void)
{
	score();
	exit(-1);
}

/*
	scoring
*/
void score(void)
{
	int t, i, k, s;
	s = t = k = 0;
	for (i = 50; i <= MAXTRS; ++i) {
		if (i == CHEST)
			k = 14;
		else if (i > CHEST)
			k = 16;
		else
			k = 12;
		if (prop[i] >= 0)
			t += 2;
		if (place[i] == 3 && prop[i] == 0)
			t += k - 2;
	}
	printf("%-20s%d\n", "Treasures:", s = t);
	t = (MAXDIE - numdie) * 10;
	if (t)
		printf("%-20s%d\n", "Survival:", t);
	s += t;
	if (!gaveup)
		s += 4;
	t = dflag ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Getting well in:", t);
	s += t;
	t = closing ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Masters section:", t);
	s += t;
	if (closed) {
		if (bonus == 0)
			t = 10;
		else if (bonus == 135)
			t = 25;
		else if (bonus == 134)
			t = 30;
		else if (bonus == 133)
			t = 45;
		printf("%-20s%d\n", "Bonus:", t);
		s += t;
	}
	if (place[MAGAZINE] == 108)
		s += 1;
	s += 2;
	printf("%-20s%d\n", "Score:", s);
}

/*
	Routine to handle the passing on of one
	of the player's incarnations...
*/
void death(void)
{
	char yea, i, j;

	if (!closing) {
		yea = yes(81 + numdie * 2, 82 + numdie * 2, 54);
		if (++numdie >= MAXDIE || !yea)
			normend();
		place[WATER] = 0;
		place[OIL] = 0;
		if (toting(LAMP))
			prop[LAMP] = 0;
		for (j = 1; j < MAXOBJ; ++j) {
			i = MAXOBJ - j;
			if (toting(i))
				drop(i, i == LAMP ? 1 : oldloc2);
		}
		newloc = 3;
		oldloc = loc;
		return;
	}
	/*
	   closing -- no resurrection...
	*/
	rspeak(131);
	++numdie;
	normend();
}

/*
	Routine to process an object.
*/
void doobj(void)
{
	/*
	   is object here?  if so, transitive
	*/
	if (fixed[object] == loc || here(object))
		trobj();
	/*
		did he give grate as destination?
	*/
	else if (object == GRATE) {
		if (loc == 1 || loc == 4 || loc == 7) {
			motion = DEPRESSION;
			domove();
		} else if (loc > 9 && loc < 15) {
			motion = ENTRANCE;
			domove();
		}
	}
	/*
		is it a dwarf he is after?
	*/
	else if (dcheck() && dflag >= 2) {
		object = DWARF;
		trobj();
	}
	/*
	   is he trying to get/use a liquid?
	*/
	else if ((liq() == object && here(BOTTLE)) || liqloc(loc) == object)
		trobj();
	else if (object == PLANT && at(PLANT2) && prop[PLANT2] == 0) {
		object = PLANT2;
		trobj();
	}
	/*
	   is he trying to grab a knife?
	*/
	else if (object == KNIFE && knfloc == loc) {
		rspeak(116);
		knfloc = -1;
	}
	/*
	   is he trying to get at dynamite?
	*/
	else if (object == ROD && here(ROD2)) {
		object = ROD2;
		trobj();
	} else
		printf("I see no %s here.\n", probj(object));
}

/*
	Routine to process an object being
	referred to.
*/
void trobj(void)
{
	if (verb)
		trverb();
	else
		printf("What do you want to do with the %s?\n", probj(object));
}

/*
	Routine to print word corresponding to object
*/
char *probj(int object)
{
	int wtype, wval;

	(void)object;
	analyze(word1, &wtype, &wval);

	return wtype == 1 ? word1 : word2;
}
/*
	dwarf stuff.
*/
void dwarves(void)
{
	int i, j, k, try, attack, stick, dtotal;

	/*
		see if dwarves allowed here
	*/
	if (newloc == 0 || forced(newloc) || cond[newloc] & NOPIRAT)
		return;
	/*
		see if dwarves are active.
	*/
	if (!dflag) {
		if (newloc > 15)
			++dflag;
		return;
	}
	/*
		if first close encounter (of 3rd kind)
		kill 0, 1 or 2
	*/
	if (dflag == 1) {
		if (newloc < 15 || pct(95))
			return;
		++dflag;
		for (i = 1; i < 3; ++i)
			if (pct(50))
				dloc[rand() % 5 + 1] = 0;
		for (i = 1; i < (DWARFMAX - 1); ++i) {
			if (dloc[i] == newloc)
				dloc[i] = daltloc;
			odloc[i] = dloc[i];
		}
		rspeak(3);
		drop(AXE, newloc);
		return;
	}
	dtotal = attack = stick = 0;
	for (i = 1; i < DWARFMAX; ++i) {
		if (dloc[i] == 0)
			continue;
		/*
			move a dwarf at random.  we don't
			have a matrix around to do it
			as in the original version...
		*/
		for (try = 1; try < 20; ++try) {
			j = rand() % 106 + 15; /* allowed area */
			if (j != odloc[i] && j != dloc[i] &&
			    !(i == (DWARFMAX - 1) && (cond[j] & NOPIRAT) == NOPIRAT))
				break;
		}
		if (j == 0)
			j = odloc[i];
		odloc[i] = dloc[i];
		dloc[i] = j;
		if ((dseen[i] && newloc >= 15) || dloc[i] == newloc || odloc[i] == newloc)
			dseen[i] = 1;
		else
			dseen[i] = 0;
		if (!dseen[i])
			continue;
		dloc[i] = newloc;
		if (i == 6)
			dopirate();
		else {
			++dtotal;
			if (odloc[i] == dloc[i]) {
				++attack;
				if (knfloc >= 0)
					knfloc = newloc;
				if (rand() % 1000 < 95 * (dflag - 2))
					++stick;
			}
		}
	}
	if (dtotal == 0)
		return;
	if (dtotal > 1)
		printf("There are %d threatening little dwarves in the room with you!\n", dtotal);
	else
		rspeak(4);
	if (attack == 0)
		return;
	if (dflag == 2)
		++dflag;
	if (attack > 1) {
		printf("%d of them throw knives at you!!\n", attack);
		k = 6;
	} else {
		rspeak(5);
		k = 52;
	}
	if (stick <= 1) {
		rspeak(stick + k);
		if (stick == 0)
			return;
	} else
		printf("%d of them get you !!!\n", stick);
	oldloc2 = newloc;
	death();
}
/*
	pirate stuff
*/
void dopirate(void)
{
	int j, k;

	if (newloc == chloc || prop[CHEST] >= 0)
		return;

	k = 0;
	for (j = 50; j <= MAXTRS; ++j)
		if (j != PYRAMID || (newloc != place[PYRAMID] && newloc != place[EMERALD])) {
			if (toting(j))
				goto stealit;
			if (here(j))
				++k;
		}
	if (tally == tally2 + 1 && k == 0 && place[CHEST] == 0 && here(LAMP) && prop[LAMP] == 1) {
		rspeak(186);
		move(CHEST, chloc);
		move(MESSAGE, chloc2);
		dloc[6] = chloc;
		odloc[6] = chloc;
		dseen[6] = 0;
		return;
	}
	if (odloc[6] != dloc[6] && pct(20)) {
		rspeak(127);
		return;
	}
	return;

stealit:

	rspeak(128);
	if (place[MESSAGE] == 0)
		move(CHEST, chloc);
	move(MESSAGE, chloc2);
	for (j = 50; j <= MAXTRS; ++j) {
		if (j == PYRAMID && (newloc == place[PYRAMID] || newloc == place[EMERALD]))
			continue;
		if (at(j) && fixed[j] == 0)
			carry(j, newloc);
		if (toting(j))
			drop(j, chloc);
	}
	dloc[6] = chloc;
	odloc[6] = chloc;
	dseen[6] = 0;
}
/*
	special time limit stuff...
*/
int stimer(void)
{
	int i;

	foobar = foobar > 0 ? -foobar : 0;
	if (tally == 0 && loc >= 15 && loc != 33)
		--clock1;
	if (clock1 == 0) {
		/*
			start closing the cave
		*/
		prop[GRATE] = 0;
		prop[FISSURE] = 0;
		for (i = 1; i < DWARFMAX; ++i)
			dseen[i] = 0;
		move(TROLL, 0);
		move((TROLL + MAXOBJ), 0);
		move(TROLL2, 117);
		move((TROLL2 + MAXOBJ), 122);
		juggle(CHASM);
		if (prop[BEAR] != 3)
			dstroy(BEAR);
		prop[CHAIN] = 0;
		fixed[CHAIN] = 0;
		prop[AXE] = 0;
		fixed[AXE] = 0;
		rspeak(129);
		clock1 = -1;
		closing = 1;
		return 0;
	}
	if (clock1 < 0)
		--clock2;
	if (clock2 == 0) {
		/*
			set up storage room...
			and close the cave...
		*/
		prop[BOTTLE] = put(BOTTLE, 115, 1);
		prop[PLANT] = put(PLANT, 115, 0);
		prop[OYSTER] = put(OYSTER, 115, 0);
		prop[LAMP] = put(LAMP, 115, 0);
		prop[ROD] = put(ROD, 115, 0);
		prop[DWARF] = put(DWARF, 115, 0);
		loc = 115;
		oldloc = 115;
		newloc = 115;
		put(GRATE, 116, 0);
		prop[SNAKE] = put(SNAKE, 116, 1);
		prop[BIRD] = put(BIRD, 116, 1);
		prop[CAGE] = put(CAGE, 116, 0);
		prop[ROD2] = put(ROD2, 116, 0);
		prop[PILLOW] = put(PILLOW, 116, 0);
		prop[MIRROR] = put(MIRROR, 115, 0);
		fixed[MIRROR] = 116;
		for (i = 1; i < MAXOBJ; ++i) {
			if (toting(i))
				dstroy(i);
		}
		rspeak(132);
		closed = 1;
		return 1;
	}
	if (prop[LAMP] == 1)
		--limit;
	if (limit <= 30 && here(BATTERIES) && prop[BATTERIES] == 0 && here(LAMP)) {
		rspeak(188);
		prop[BATTERIES] = 1;
		if (toting(BATTERIES))
			drop(BATTERIES, loc);
		limit += 2500;
		lmwarn = 0;
		return 0;
	}
	if (limit == 0) {
		--limit;
		prop[LAMP] = 0;
		if (here(LAMP))
			rspeak(184);
		return 0;
	}
	if (limit < 0 && loc <= 8) {
		rspeak(185);
		gaveup = 1;
		normend();
	}
	if (limit <= 30) {
		if (lmwarn || !here(LAMP))
			return 0;
		lmwarn = 1;
		i = 187;
		if (place[BATTERIES] == 0)
			i = 183;
		if (prop[BATTERIES] == 1)
			i = 189;
		rspeak(i);
		return 0;
	}
	return 0;
}
