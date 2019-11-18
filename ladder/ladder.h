// Word ladder lander lender leader header.
// (c) years of the sheep, monkey, rooster, and pig, Duane
#ifndef LADDER_H
#define LADDER_H
/*
 * This is the default dictionary; you can specify an alternative
 * on the command line; viz. ladder -d knuth
 */
#define DICT "/usr/share/dict/words"

// maximum word length (college-level)
#define MAXL 32

// Hydrolic, barrel-sifted error monger:
#define YOWZA(c,...) if (c) { fprintf(stderr,"%s:%d:Yikes! ",__FILE__,__LINE__);fprintf(stderr,__VA_ARGS__); exit(1); }

// forward declaration of types:
typedef struct _ent entry;
typedef struct _dic dict;
typedef struct _q queue;

// "looker-upper": technical term for "binary search function"
typedef entry *lookerUpper(char *, dict *);

// extensible dictionary
struct _dic {
  int    alloced;	// number allocated
  entry *dict;		// the entries
  int    count;		// number used
};

// word definition (see ladder.c for explanification)
struct _ent {
  int seen;		// has this word been considered in this search
  char *word;		// the word (dynamically allocated)
  entry *que;	        // what is the next word in the BFS from the source
  entry *ladd;	        // what is the previous word in the word ladder
};

// Queue administration:
struct _q {
  entry *head;
  entry *tail;
};


/*
 * Code written/to-be-written, all in 'betical order:
 */
extern void   bfs(entry *src,entry *dest, dict *dtn);
extern entry *deq(queue *q);
extern int    empty(queue *q);
extern void   enq(entry *e, queue *q);
extern lookerUpper isWordC;
extern lookerUpper isWordAsm;
extern queue *newQ();
extern void   parseArgs(int argc, char **argv);
extern int    printLadder(entry *i, int n);
extern dict  *readDict(char *fn);
extern void   Usage(char *name);
#endif
