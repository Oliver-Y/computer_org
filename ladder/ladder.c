// Word ladders: wave->wive->wine->wind; motivated by Lewis Carroll and Google.
// (c) 2015-2019 duane a. bailey

/*
 * This is a standard, breadth-first-search approach to finding a path or
 * 'ladder' from a source word to a destination word.
 *
 * Breadth first search uses a queue to keep track of the progress of the
 * expanding frontier of the search.  In order to avoid re-considering search
 * space we also need to keep track of progress with some means of marking
 * nodes as *visited*.  Finally, when the target of the search is encountered
 * we typically try to recover a fast path (here, the word ladder) from the
 * source to target.  An easy technique is to have nodes keep track of
 * predecessors (this is best; there are potentially many successors).
 *
 * Instead of implementing many dynamic structures, we choose here to store
 * words (nodes in search space) in a table of dictionary *entries*.  Each
 * entry contains a pointer to the word (->word), whether it has been
 * encountered during this search (->seen), a pointer to the next word in the
 * queue of words to be investigated (->next), and a pointer to the previous
 * word in the word ladder (->ladd).  In this way, the compact structure keeps
 * track of an ordered dictionary, a queue of to-be-investigated words, and
 * the resulting word ladder, all with little dynamic memory overhead.
 *
 * This is gonna be good.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// common declarations
#include "ladder.h"

// 'static' items are 'not global'; they're local
static char *dictName = DICT;
static char *srcWord = 0;
static char *dstWord = 0;

// STUDENTS: redirect search mechanism here:
lookerUpper *dictSearch = isWordC; // or: isWordAsm

/* STUDENTS:
 * Search for and return pointer to word (entry) in dictionary.
 * Assumes dictionary entries are in sorted order.
 * Return 0 if w is not a word.
 */
entry *isWordC(char *w, dict *dtn)
{
  // Binary search!
  int lo = 0, hi = (dtn->count)-1;    // remaining candidates
  int mid,dif;			      // middle and difference with w
  entry *ent, *ans=0;		      // ans: entry if found; else 0
  while (lo <= hi) {
    mid = (lo+hi)/2;		      // entry in middle of candidates
    ent = (dtn->dict)+mid;	      // warning: sharp curve!
    dif = strcmp(w,ent->word);	      // effectively 'w minus word':
    if (dif < 0) { hi = mid-1; }      // this entry too big,
    else if (dif > 0) { lo = mid+1; } // this entry too small, or
    else { ans = ent; break; }	      // this entry just right
  }
  return ans; // inform the masses
}

/*
 * Read in a dictionary of words, one per line.
 * Proper names (those starting with uppercase) are silently dropped.
 * Result is a dict structure.
 */
dict *readDict(char *fn)
{
  char buffer[MAXL];
  dict *dtn = (dict*)calloc(1,sizeof(dict)); // N.B. initialized to 0's

  // allocate the dictionary structure
  dtn->alloced = 10;
  dtn->count = 0;
  dtn->dict = (entry*)calloc(dtn->alloced,sizeof(entry));

  // open and read file
  FILE *d = fopen(fn,"r");
  YOWZA(!d,"Dictionary '%s' not readable.\n",fn);
  while (fgets(buffer,MAXL,d)) {
    int l = strlen(buffer);
    // strip trailing yuck off
    while (!isalpha(buffer[l-1])) {
      buffer[--l] = 0;
    }
    // ignore proper nouns
    if (isupper(buffer[0])) continue;
    // check to see if vector needs extending:
    if (dtn->alloced == dtn->count) {
      dtn->alloced *= 2;
      dtn->dict = (entry*)realloc(dtn->dict,dtn->alloced*sizeof(entry));
      assert(dtn->dict);
    }
    // add in the word
    entry *w = dtn->dict+(dtn->count++);
    w->word = strdup(buffer);
    w->que = w->ladd = 0;
    w->seen = 0;
  }
  // close up and return
  fclose(d);
  return dtn;
}


/*
 * Create a new queue.
 */
queue *newQ()
{
  queue *result = (queue*)calloc(1,sizeof(queue));
  YOWZA(!result,"Could not allocate queue.\n");
  return result;
}

/*
 * Return: Is queue empty?
 */
int empty(queue *q)
{
  return !q->head;
}

/*
 * Add new entry to queue; mark as seen.
 */
void enq(entry *e, queue *q)
{
  e->seen = 1;
  if (q->head == 0) {
    q->head = q->tail = e;
  } else {
    q->tail->que = e;
    q->tail = e;
  }
}

/*
 * If possible, dequeue a value from queue.
 */
entry *deq(queue *q)
{
  entry *result = 0;
  if (q->head) {
    result = q->head;
    q->head = result->que;
    if (q->head == 0) q->tail = 0;
  }
  return result;
}

/*
 * Print the word ladder, starting at end.
 * You loved recursion as a kid.
 * Live like a kid.
 */
int printLadder(entry *i, int n)
{
  if (i->ladd) n = printLadder(i->ladd, n);
  printf("%d. %s\n",n, i->word);
  return n+1;
}

/*
 * Perform breadth first search from src toward dest.
 * Stops when dest is reached or search is completed.
 */
void bfs(entry *src,entry *dest, dict *dtn)
{
  char temp[MAXL];
  int l = strlen(src->word);
  int i,c2;
  char c;

  queue *q = newQ();
  enq(src,q);
  while (!empty(q)) {
    entry *w = deq(q);
    if (w == dest) return;
    // process word:
    strcpy(temp,w->word);
    for (i = 0; i < l; i++) {
      c = temp[i];
      for (c2 = 'a'; c2 <= 'z'; c2++) {
	if (c == c2) continue; // sub an 'x' with 'x'? skip
	temp[i] = c2;
	entry *nextWord = dictSearch(temp,dtn);
	if (!nextWord) continue;
	if (nextWord->seen)continue;
	nextWord->ladd = w;
	enq(nextWord,q);
      }
      temp[i] = c;
    }
  }
}

/*
 * Print. help. and. die.
 */
void Usage(char *name)
{
  fprintf(stderr,"Usage: %s [-d dict] begin end\n",name);
  fprintf(stderr,"\t-d\tuse dictionary dict (currently %s)\n",dictName);
  fprintf(stderr,"\tbegin\tstart word in ladder\n");
  fprintf(stderr,"\tend\tlast word in ladder\n");
  exit(1);
}

/*
 * Look for arguments from user.
 */
void parseArgs(int argc, char **argv)
{
  char *prog = *argv++; argc--;		// grab program name
  while (argc) {
    char *arg = *argv++; argc--;
    if (strcmp(arg,"-d") == 0) {	// set alternative dictionary
      dictName = *argv++; argc--;
    } else {
      if (!srcWord) { srcWord = arg; }		// source word
      else if (!dstWord) { dstWord = arg; }	// target word
      else Usage(prog);
    }
  }
  if (!(srcWord && dstWord)) Usage(prog);		// detect twit
}

int main(int argc, char **argv)
{
  dict *wordList;	// the dictionary
  entry *src, *dst;	// ends of word ladder

  parseArgs(argc, argv);
  wordList = readDict(dictName);
  YOWZA(strlen(srcWord) != strlen(dstWord),"Words must be the same length.\n");
  src = dictSearch(srcWord,wordList);
  YOWZA(!src,"Word '%s' is not in dictionary.\n",srcWord);
  dst = dictSearch(dstWord,wordList);
  YOWZA(!dst,"Word '%s' is not in dictionary.\n",dstWord);
  bfs(src,dst,wordList);
  if (dst->seen) {
    printLadder(dst,0);
  } else {
    puts("No ladder between words.");
  }
  return 0;
}

