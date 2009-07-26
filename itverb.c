
/*	program ITVERB.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h".		*/


#include	<stdio.h>	/* drv = 1.1st file 2.def 3.A	*/
#include	"advent.h"
#include	"advdec.h"


/*
	Routines to process intransitive verbs
*/
itverb()
{
	switch(verb) {
	case DROP:
	case SAY:
	case WAVE:
	case CALM:
	case RUB:
	case THROW:
	case FIND:
	case FEED:
	case BREAK:
	case WAKE:
		needobj();
		break;
	case TAKE:
		ivtake();
		break;
	case OPEN:
	case LOCK:
		ivopen();
		break;
	case NOTHING:
		rspeak(54);
		break;
	case ON:
	case OFF:
	case POUR:
		trverb();
		break;
	case WALK:
		actspk(verb);
		break;
	case KILL:
		ivkill();
		break;
	case EAT:
		iveat();
		break;
	case DRINK:
		ivdrink();
		break;
	case QUIT:
		ivquit();
		break;
	case FILL:
		ivfill();
		break;
	case BLAST:
		vblast();
		break;
	case SCORE:
		score();
		break;
	case FOO:
		ivfoo();
		break;
	case SUSPEND:
		saveflg = 1;
		break;
	case INVENTORY:
		inventory();
		break;
	case LOAD:
		restore();
		break;
	default:
		printf("This intransitive not implemented yet\n");
	}
}

/*
	CARRY, TAKE etc.
*/
ivtake()
{
	int anobj,item;

	anobj = 0;
	for(item=1;item<MAXOBJ;++item) {
		if(place[item]==loc) {
			if(anobj != 0) {
				needobj();
				return;
			}
			anobj = item;
		}
	}
	if(anobj==0|| (dcheck() && dflag>=2)) {
		needobj();
		return;
	}
	object = anobj;
	vtake();
}

/*
	OPEN, LOCK, UNLOCK
*/
ivopen()
{
	if(here(CLAM))
		object=CLAM;
	if(here(OYSTER))
		object=OYSTER;
	if(at(DOOR))
		object=DOOR;
	if(at(GRATE))
		object=GRATE;
	if(here(CHAIN)) {
		if(object != 0) {
			needobj();
			return;
		}
		object=CHAIN;
	}
	if(object==0) {
		rspeak(28);
		return;
	}
	vopen();
}

/*
	ATTACK, KILL etc
*/
ivkill()
{
	object1 = 0;
	if(dcheck() && dflag >=2)
		object=DWARF;
	if(here(SNAKE))
		addobj(SNAKE);
	if(at(DRAGON) && prop[DRAGON]==0)
		addobj(DRAGON);
	if(at(TROLL))
		addobj(TROLL);
	if(here(BEAR) && prop[BEAR]==0)
		addobj(BEAR);
	if(object1 != 0) {
		needobj();
		return;
	}
	if(object != 0) {
		vkill();
		return;
	}
	if(here(BIRD) && verb!= THROW)
		object=BIRD;
	if(here(CLAM) || here(OYSTER))
		addobj(CLAM);
	if(object1 != 0) {
		needobj();
		return;
	}
	vkill();
}

/*
	EAT
*/
iveat()
{
	if(!here(FOOD))
		needobj();
	else {
		object=FOOD;
		veat();
	}
}

/*
	DRINK
*/
ivdrink()
{
	if(liqloc(loc) != WATER &&
	  (liq()!= WATER || !here(BOTTLE)))
		needobj();
	else {
		object=WATER;
		vdrink();
	}
}

/*
	QUIT
*/
ivquit()
{
	if(gaveup=yes(22,54,54))
		normend();
}

/*
	FILL
*/
ivfill()
{
	if(!here(BOTTLE))
		needobj();
	else {
		object=BOTTLE;
		vfill();
	}
}

/*
	Handle fee fie foe foo...
*/
ivfoo()
{
	char k,msg;
	k = vocab(word1,3000);
	msg = 42;
	if (foobar != 1-k) {
		if (foobar != 0)
			msg = 151;
		rspeak(msg);
		return;
	}
	foobar = k;
	if (k != 4)
		return;
	foobar = 0;
	if (place[EGGS] == 92 ||
	   (toting(EGGS) && loc == 92)) {
		rspeak(msg);
		return;
	}
	if (place[EGGS] == 0 && place[TROLL] == 0 &&
	    prop[TROLL] == 0)
		prop[TROLL] = 1;
	if (here(EGGS))
		k = 1;
	else if (loc == 92)
		k = 0;
	else
		k = 2;
	move(EGGS,92);
	pspeak(EGGS,k);
	return;
}

/*
	read etc...
*/
/*  no room for this...
ivread()
{
	if (here(MAGAZINE))
		object = MAGAZINE;
	if (here(TABLET))
		object = object*100 + TABLET;
	if (here(MESSAGE))
		object = object*100 + MESSAGE;
	if (object > 100 || object == 0 || dark()) {
		needobj();
		return;
	}
	vread();
}
*/

/*
	INVENTORY 
*/
inventory()
{
	char msg;
	int i;

	msg = 98;
	for (i=1; i<=MAXOBJ; ++i) {
		if (i==BEAR || !toting(i))
			continue;
		if (msg)
			rspeak(99);
		msg = 0;
		pspeak(i,-1);
	}
	if (toting(BEAR))
		msg = 141;
	if (msg)
		rspeak(msg);
}

/*
	ensure uniqueness as objects are searched
	out for an intransitive verb
*/
addobj(obj)
{
	if(object1 != 0)
		return;
	if(object != 0) {
		object1 = -1;
		return;
	}
	object = obj;
}


