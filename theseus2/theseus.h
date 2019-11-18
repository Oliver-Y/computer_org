// Definitions important to theseus.
// (c) 2015-2019 duane a. bailey
#ifndef THESEUS_H
#define THESEUS_H
#include <curses.h>

// Dimensions of screen, as reported by curses
#define MAXROWS LINES
#define MAXCOLS COLS

/*
 * The level structure.
 * This can be improved.  In particular, it might be useful to have
 * the undo stack and statistics within this structure.
 */
typedef struct level_st {
  int levelNumber; // difficulty (0-MAXLEVEL)
  int rows, cols;  // dimensions of level
  char **pic;      // level lines (use get(l,r,c) to get elements)
  int top, left;   // margin sizes
  int theseus;     // position of our hero
  int minotaur;    // position of his nemesis
  int goal;        // position of goal
  long int startTime; // the date we began playing
} level;

typedef struct movement {
  char player;
  int pos;
} movement;

// Constants of the universe
#define PAUSE 0
#define MINOTAUR_SPEED 2

// Bits representing maze locations in l->pic (or-ed together)
#define LEFT 1
#define UP   2
#define RIGHT 4
#define DOWN 8
#define THESEUS 16
#define MINOTAUR 32
#define GOAL 64

// Location of important files:
#define SCREENLOC "levels/%02d.lvl"
#define HELPSCREEN "levels/HELP"
#define OMGSCREEN "levels/WORK"

// Number of different puzzle levels
// (you can start theseus at a particular level with theseus <levelnumber>)
#define MAXLEVEL 87

// For computing control-key values: clear the upper bits of ASCII
#define CTRL(ch) (ch&~(128|64|32))

// Magic phrase at the beginning of each level:
#define MAGIC "Theseus and the Minotaur Level\n"

// global variables:
extern int BDay[];     // date you were born
extern long BTime;     // when you were born, seconds since 1970
extern int MaxStore;   // initial allocation for storage index array
extern int MaxUndo;    // initial allocation for undo stack (silly)
extern movement *UndoStack;      // the undo stack
extern int UndoTop;    // top of the stack; points to last move
extern int MoveCount;   // number of moves

/*
 * Forward declaration of functions.
 * (The extern keyword means "if code not found here, the look outside".)
 */

// (see documentation in theseus.c)
extern int chase(level *l, int strides);
extern void die(char *why);
extern void display(level *l);
extern char get(level *l, int row, int col);
extern int getM(level *l);
extern int getT(level *l);
extern int go(level *l, int direction);
extern int height(level *l);
extern void help();
extern void initialize();
extern int lose(level *l);
extern void message(char *msg);
extern int moveM(level *l);
extern void movePiece(level *l, int player, int r0, int c0, int r1, int c1);
extern void movePieceQ(level *l, int player, int r0, int c0, int r1, int c1);
extern void mvstr(int r, int c, char *s);
extern void OMG(level *l);
extern void p2rc(int p, int *r, int *c);
extern int play(level *);
extern void pushMove(int m, int player);
extern int rc2p(int r, int c);
extern level *readlevel(int n);
extern void shutdown();
extern int undo(level *l);
extern void update(level*l, int r, int c);
extern void updateStats(level *l);
extern int wallPic(level *l, int r, int c);
extern int width(level *l);
extern int win(level *l);
extern void work();
#endif
