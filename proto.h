#ifdef __STDC__
# define	P(s) s
#else
# define P(s) ()
#endif


/* advent.c */
/*      37 */ int main P((int argc , char **argv ));
/*      89 */ int scanint P((int *pi , char *str ));
/*     106 */ int initplay P((void ));
/*     180 */ int opentxt P((void ));
/*     207 */ int saveadv P((void ));
/*     244 */ int restore P((void ));
char *game_name P((char *username));

/* database.c */
/*      25 */ int gettrav P((int loc ));
/*      62 */ int rdupto P((FILE *fdi , int uptoc , int print , char *string ));
/*      89 */ int rdskip P((FILE *fdi , int skipc , int n , int rewind ));
/*     108 */ int yes P((int msg1 , int msg2 , int msg3 ));
/*     130 */ int rspeak P((int msg ));
/*     147 */ int pspeak P((int item , int state ));
/*     158 */ int desclg P((int loc ));
/*     168 */ int descsh P((int loc ));
/*     184 */ int vocab P((char *word , int val ));
/*     208 */ int binary P((char *w , struct wac wctable [], int maxwc ));
/*     237 */ int dark P((void ));
/*     247 */ int here P((int item ));
/*     256 */ int toting P((int item ));
/*     266 */ int forced P((int atloc ));
/*     275 */ int pct P((int x ));
/*     285 */ int at P((int item ));
/*     294 */ int dstroy P((int obj ));
/*     303 */ int move P((int obj , int where ));
/*     318 */ int juggle P((int loc ));
/*     326 */ int carry P((int obj , int where ));
/*     340 */ int drop P((int obj , int where ));
/*     357 */ int put P((int obj , int where , int pval ));
/*     367 */ int dcheck P((void ));
/*     380 */ int liq P((void ));
/*     391 */ int liqloc P((int loc ));
/*     405 */ int liq2 P((int pbottle ));
/*     414 */ int bug P((int n ));

/* english.c */
/*      21 */ int english P((void ));
/*     100 */ int analyze P((char *word , int *type , int *value ));
/*     130 */ int getwords P((void ));
/*     150 */ int outwords P((void ));

/* itverb.c */
/*      16 */ int itverb P((void ));
/*      91 */ int ivtake P((void ));
/*     116 */ int ivopen P((void ));
/*     143 */ int ivkill P((void ));
/*     193 */ int ivdrink P((void ));
/*     207 */ int ivquit P((void ));
/*     231 */ int ivfoo P((void ));
/*     288 */ int inventory P((void ));
/*     312 */ int addobj P((int obj ));

/* turn.c */
/*      18 */ int turn P((void ));
/*     116 */ int describe P((void ));
/*     133 */ int descitem P((void ));
/*     165 */ int domove P((void ));
/*     199 */ int goback P((void ));
/*     242 */ int copytrv P((struct trav *trav1 , struct trav *trav2 ));
/*     258 */ int dotrav P((void ));
/*     326 */ int badmove P((void ));
/*     344 */ int spcmove P((int rdest ));
/*     396 */ int dwarfend P((void ));
/*     405 */ int normend P((void ));
/*     414 */ int score P((void ));
/*     467 */ int death P((void ));
/*     499 */ int doobj P((void ));
/*     560 */ int trobj P((void ));
/*     572 */ char *probj P((int object ));
/*     581 */ int dwarves P((void ));
/*     688 */ int dopirate P((void ));
/*     742 */ int stimer P((void ));
#ifndef __QNX__
/*     848 */ int srand P((int n ));
/*     857 */ int rand P((void ));
#endif

/* verb.c */
/*      16 */ int trverb P((void ));
/*     106 */ int vtake P((void ));
/*     182 */ int vdrop P((void ));
/*     271 */ int vopen P((void ));
/*     357 */ int vsay P((void ));
/*     368 */ int von P((void ));
/*     387 */ int voff P((void ));
/*     400 */ int vwave P((void ));
/*     417 */ int vkill P((void ));
/*     484 */ int vpour P((void ));
/*     523 */ int veat P((void ));
/*     546 */ int vdrink P((void ));
/*     562 */ int vthrow P((void ));
/*     658 */ int vfind P((void ));
/*     681 */ int vfill P((void ));
/*     721 */ int vfeed P((void ));
/*     780 */ int vread P((void ));
/*     816 */ int vblast P((void ));
/*     834 */ int vbreak P((void ));
/*     861 */ int vwake P((void ));
/*     874 */ int actspk P((int verb ));
/*     891 */ int needobj P((void ));

#undef P
