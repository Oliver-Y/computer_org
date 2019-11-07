/*
 * A curses-based implementation of Robert Abbott's Theseus & Minotaur Puzzle.
 * (c) 2015-2019 duane a. bailey
 *
 * See screens/HELP, or type '?' in the running game for help on the puzzle.
 * See man ncurses for help understanding the curses interface.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "theseus.h"

//
// Global variables.
//
int MaxStore = 10;   // initial allocation for storage index array
int MaxUndo = 10;    // initial allocation for undo stack (silly)
movement *UndoStack;      // the undo stack
int UndoTop = -1;    // top of the stack; points to last move
int MoveCount = 0;   // number of moves
int HeroSteps = 0;
long BTime;          // when you were born
int currentLevelNumber;

/****************************************************************************
 * Code
 */
// initialize the screen; called once.
void initialize()
{
  struct tm bd[1];
  // curses initialization:
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr,TRUE);
  // initial undo stack allocation
  UndoStack = (movement*)malloc(MaxUndo*sizeof(movement));

  // birthday determination for biometrics
  bd->tm_sec = bd->tm_min = bd->tm_hour = 0;
  bd->tm_mday = BDay[1];
  bd->tm_mon = BDay[0]-1;
  bd->tm_year = BDay[2]-1900;
  BTime = mktime(bd);

  message("Welcome to Theseus -- type '?' for help, '^G' to quit.");
}

void die(char *msg)
{
  shutdown();  // we need to reset the terminal, cleanly
  fputs(msg,stderr);
  exit(1);
}

// Read level n from a screen file (0 to 87).
// This program starts at level 1, with 0 for experimentation.
// Select the initial level at the command line; e.g. theseus 0
level *readLevel(int n)
{
  FILE *lf;
  char levelName[80];
  level *result;
  char buffer[80];
  int r, c;

  // attempt to open the level
  sprintf(levelName,SCREENLOC,n);
  lf = fopen(levelName,"r");
  if (lf == 0) die("Could not open level file.\n");

  // Ok, we're good to allocate structures and read level
  result = (level *)malloc(sizeof(level));
  assert(result);
  result->levelNumber = n;
  result->rows = 0;
  result->cols = 0;
  result->pic = (char**)malloc(MAXROWS*sizeof(char*));
  result->theseus = result->minotaur = 0;

  // read in magic statement at beginning of file
  if (fgets(buffer,80,lf) == 0) die("Level file is corrupt.\n");
  if (strcmp(buffer,MAGIC)) die("Level file has bad magic.\n");
  // read in the rows
  int keep = 1;
  while (fgets(buffer,80,lf)) {
    char *bp = buffer;
    if (!isdigit(buffer[0])) break;
    if (!keep) continue;
    while (*bp >= '0' && *bp < '6') bp++;
    *bp = '\0';
    int l = bp - buffer;
    if (l == 0 && result->rows > 0) {
      keep = 0;
      continue;
    }
    // copy into picture array
    result->pic[result->rows] = strdup(buffer);
    result->rows++;
    if (l > result->cols) result->cols = l;
  }

  // make it just the right size
  result->pic = (char **)realloc(result->pic, result->rows*sizeof(char*));

  int row,col;
  assert(strcmp(buffer,"Theseus\n")==0);
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&col));
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&row));
  result->theseus = rc2p(row,col);

  assert(fgets(buffer,80,lf));
  assert(strcmp(buffer,"Minotaur\n")==0);
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&col));
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&row));
  result->minotaur = rc2p(row,col);

  assert(fgets(buffer,80,lf));
  assert(strcmp(buffer,"Goal\n")==0);
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&col));
  assert(fgets(buffer,80,lf));
  assert(1 == sscanf(buffer,"%d",&row));
  result->goal = rc2p(row,col);

  assert(fgets(buffer,80,lf));
  assert(strcmp(buffer,"End\n")==0);
  assert(fgets(buffer,80,lf));
  int level;
  assert(1 == sscanf(buffer,"%d",&level));
  //assert(level == n);

  for (r = 0; r < result->rows; r++)
    for (c = 0; c < result->cols; c++)
      result->pic[r][c] -= '0';

  for (r = 0; r < result->rows; r++) {
    for (c = 0; c < result->cols; c++) {
      int p = rc2p(r,c);
      char ch = result->pic[r][c];
      if (r == 0) ch |= UP;
      else if (ch & UP) result->pic[r-1][c] |= DOWN;
      if (c == 0) ch |= LEFT;
      else if (ch & LEFT) result->pic[r][c-1] |= RIGHT;
      if (r == result->rows-1) ch |= DOWN;
      else if (ch & DOWN) result->pic[r+1][c] |= UP;
      if (c == result->cols-1) ch |= RIGHT;
      else if (ch & RIGHT) result->pic[r][c+1] |= LEFT;
      
      if (p == result->theseus) ch |= THESEUS;
      if (p == result->minotaur) ch |= MINOTAUR;
      if (p == result->goal) ch |= GOAL;
      result->pic[r][c] = ch;
    }
  }

  // record the time we start this level
  result->startTime = time(0);

  // you will note that MoveCount and UndoTop are related; they needn't be
  // reset the stack top
  UndoTop = -1;
  // reset the move counter
  HeroSteps = MoveCount = 0;
  shutdown();
  return result;
}

// display the current level in its current state
void display(level*l)
{
  int r, c, i;

  // set these for later use: these margins are computed
  l->top = (MAXROWS-(2*l->rows+1))/2;
  l->left = (MAXCOLS-(2*l->cols+1))/2;

  // clear the screen (faster way? see man ncurses)
  for (i = 1; i < MAXROWS; i++) {
    move(i,0); clrtoeol();
  }

  // draw the level
  for (r = 0; r < l->rows; r++) {
    //char *line = l->pic[r];
    // we're always worried: the lines are variable length
    int lineLen = l->cols;
    for (c = 0; c < lineLen; c++) {
      // update is responsible for determining the correct representation
      update(l,r,c);
    }
  }
  // update biometric stats
  updateStats(l);
  // repaint screen
  refresh();
}

// go through the motions of play
// read in a key and act on it
// most controls follow the emacs movement keys.  we also support ^U for
// multiplying the power of commands
int play(level *l)
{
  int done = 0;        // true when finished
  int repeatCount = 0; // number of outstanding times to repeat key
  int prefix = 0;      // true if the key can't be repeated

  while (!done) {
    prefix = 0;
    int ch = getch();
    do {
      switch (ch) {
	// basic motion: take emacs or, god forbid, arrow keys
      case CTRL(' '): if (!go(l,PAUSE)) repeatCount = 0; break;
      case CTRL('B'): if (!go(l,LEFT)) repeatCount = 0; break;
      case CTRL('F'): if (!go(l,RIGHT)) repeatCount = 0; break;
      case CTRL('N'): if (!go(l,DOWN)) repeatCount = 0; break;
      case CTRL('P'): if (!go(l,UP)) repeatCount = 0; break;

	// undo last move
      case CTRL('_'): if (!undo(l)) repeatCount = 0; break;

	// the boss key (not repeatable)
      case ' ': OMG(l); repeatCount = 0; break;

	// the help key (not repeatable)
      case '?': help(l); repeatCount = 0; break;

	// the repeat key (sets the repeat count to 4, or 4*repeat count)
      case CTRL('U'):
	if (repeatCount < 1) repeatCount = 4;
	else repeatCount *= 4;
	prefix = 1;
	break;

	// loser key: quit puzzle
      case CTRL('G'):
	  shutdown();
	  exit(0);

	// otherwise, silently do nothing
      default:
	break;
      }

      // check for win; if a win, indicate message, read a key, end play
      if (lose(l)) {
	updateStats(l);
	message("YOU LOSE! (Press 'r' for restarting this level.)");
	refresh();
	while ('r' != getch());
	repeatCount = 0;
	done = 1;
      } else if (win(l)) {
	updateStats(l);
	message("YOU WIN! (Press 'g' for next level.)");
	refresh();
	while ('g' != getch());
	repeatCount = 0;
	done = 1;
	currentLevelNumber++;
      } else {
	updateStats(l);
	refresh();
	if (!prefix) if (repeatCount) repeatCount--;
      }
    } while (repeatCount && !prefix);
  }
  // end of level: clear the screen
  clear();
  return 1;
}

// move theseus in the indicated direction
int go(level *l, int direction)
{
  int r, c; // row and column of theseus
  int dr = 0, dc = 0; // delta row, col
  int sr, sc; // space row, col
  int sch;
  int moved = 0; // return true if the theseus was successful in move

  // first, convert direction to a change in row and column:
  switch (direction) {
    case UP: dr = -1; break;
    case RIGHT: dc = 1; break;
    case DOWN: dr = 1; break;
    case LEFT: dc = -1; break;
  }
  // get location
  p2rc(l->theseus,&r,&c);
  // compute hoped-for space location
  sr = r+dr; sc = c+dc;
  // check for space
  sch = get(l,r,c);
  if (!(sch & direction)) {
    // we can move the Theseus from (r,c) to (sr,sc)
    movePiece(l,THESEUS,r,c,sr,sc);    
    // record the fact we moved
    moved = 1;
    if (dr || dc) HeroSteps++;
  }
  if (moved) {
    MoveCount++;
    chase(l,MINOTAUR_SPEED);
  }
  return moved;
}


// curses rundown resets terminal to function normally
// must be called on any exit
void shutdown()
{
  endwin();
}

/**************************************************************************
 * Utility methods
 */
// get Theseus position: low two nibbles 0x000000RC
int getT(level *l)
{
  return l->theseus;
}

// get Minotaur position: low two nibbles 0x000000RC
int getM(level *l)
{
  return l->minotaur;
}

// convert row and column to a position
int rc2p(int r, int c)
{
  return (r<<4)|c;
}

// convert a position to a row and column
void p2rc(int p, int *r, int *c)
{
  *r = (p >> 4) & 0xf;
  *c = p & 0xf;
}

// return the width of the level
int width(level *l)
{
  return l->cols;
}

// return the height of the level
int height(level *l)
{
  return l->rows;
}

// get the descriptor of the cell at [row,col] in level l
char get(level *l, int row, int col)
{
  int len,high;
  high = l->rows;
  len =  l->cols;
  if ((0 <= row) && (row < high) && (0 <= col) && (col < len)) {
    return l->pic[row][col];
  } else {
    char result = 0;
    if (row < 0 && ((0 <= col) && (col < len))) result |= DOWN;
    if (row >= high && ((0 <= col) && (col < len))) result |= UP;
    if (col < 0 && ((0 <= row) && (row < high))) result |= RIGHT;
    if (col >= len && ((0 <= row) && (row < high))) result |= LEFT;
    return result;
  }
}


// Determine the symbol needed to represent this portion of wall
int wallPic(level *l, int r, int c)
{
  // we walk around from North (1), to East (2), South (4), and West (8) and we
  // keep track of all the surrounding walls.  From this, we pick the
  // different symbols that determine the wall character here, at [r,c]
  int pattern = 0;
  int border[] = { ' ', ACS_VLINE, ACS_HLINE, ACS_LRCORNER,
                   ACS_HLINE, ACS_LLCORNER, ACS_HLINE, ACS_BTEE,
		   ACS_VLINE, ACS_VLINE, ACS_URCORNER, ACS_RTEE,
		   ACS_ULCORNER, ACS_LTEE, ACS_TTEE, ACS_PLUS};
  // Fix!
  if (RIGHT & get(l,r,c)) pattern |= 1;
  if (DOWN & get(l,r,c)) pattern |= 2;
  if (UP & get(l,r+1,c+1)) pattern |= 4;
  if (LEFT & get(l,r+1,c+1)) pattern |= 8;
  return border[pattern];
}

// update (possible) changes to what appears on the screen at [r,c]
void update(level*l, int r, int c)
{
  int ch = l->pic[r][c];
  int ouch = '.';
  int or = r;
  int oc = c;
  r = r*2 + 1 + l->top;
  c = c*2 + 1 + l->left;
  // wowza; not beautiful
  mvaddch(r-1,c-1,wallPic(l,or-1,oc-1));
  mvaddch(r+1,c-1,wallPic(l,or,oc-1));
  mvaddch(r+1,c+1,wallPic(l,or,oc));
  mvaddch(r-1,c+1,wallPic(l,or-1,oc));
  mvaddch(r,c-1,(ch&LEFT)?ACS_VLINE:' ');
  mvaddch(r,c+1,(ch&RIGHT)?ACS_VLINE:' ');
  mvaddch(r-1,c,(ch&UP)?ACS_HLINE:' ');
  mvaddch(r+1,c,(ch&DOWN)?ACS_HLINE:' ');
  if (THESEUS & ch) ouch = 't';
  if (MINOTAUR & ch) ouch = 'm';
  if (GOAL & ch) ouch = '@';
  if ((THESEUS & ch) && (GOAL & ch)) ouch = 'T';
  if ((MINOTAUR & ch) && (GOAL & ch)) ouch = 'M';
  if ((THESEUS & ch) && (MINOTAUR & ch)) ouch = '*';

  mvaddch(r,c,ouch);
}

// move a piece at [r0,c0] to empty space [r1,c1]
void movePiece(level *l, int player, int r0, int c0, int r1, int c1)
{
  int p;
  movePieceQ(l,player,r0,c0,r1,c1);
  p = rc2p(r0,c0);
  pushMove(p,player);
}

// quick move a piece at [r0,c0] to empty space [r1,c1], without recording
void movePieceQ(level *l, int player, int r0, int c0, int r1, int c1)
{
  int r, c;
  int ch0,ch1;
  // get location to see if theseus is getting moved
  if (player == THESEUS) {
    p2rc(l->theseus,&r,&c);
    if (r0 == r && c0 == c) {
      l->theseus = rc2p(r1,c1);
    } // if so, update pos
  }
  if (player == MINOTAUR) {
    // get location to see if minotaur is getting moved
    p2rc(l->minotaur,&r,&c);
    if (r0 == r && c0 == c) {
      l->minotaur = rc2p(r1,c1);
    } // if so, update pos
  }

  ch0 = l->pic[r0][c0]; // character at source
  ch1 = l->pic[r1][c1]; // character at destination

  // clear player bit at source
  l->pic[r0][c0] = (ch0 & ~player);
  // copy player bit to the destination
  l->pic[r1][c1] = (ch1 & ~player) | (ch0 & player);
  // repaint both
  update(l,r0,c0);
  update(l,r1,c1);
}

// push move onto the undo stack, possibly extending it
void pushMove(int m, int p)
{
  UndoTop = UndoTop+1;
  if (UndoTop >= MaxUndo) {
    MaxUndo *= 2;
    UndoStack = (movement*)realloc(UndoStack,MaxUndo*sizeof(movement));
  }
  // store move on stack top
  UndoStack[UndoTop].pos = m;
  UndoStack[UndoTop].player = p;
}

// back up one move
int undo(level *l)
{
  int p;
  int player;
  int sr,sc;
  int r,c;

  do {
    if (UndoTop < 0) return 0;
  
    // grab last move
    p = UndoStack[UndoTop].pos;
    player = UndoStack[UndoTop].player;
    UndoTop--;

    // move the player to location p
    // move player into space
    if (player == THESEUS) p2rc(l->theseus, &r, &c);
    else p2rc(l->minotaur, &r, &c);
    p2rc(p, &sr, &sc);
    movePieceQ(l,player,r,c,sr,sc);
    // update biostatistics
    updateStats(l);
    refresh();
  } while (player != THESEUS);
  // fix move count
  if (r != sr || c != sc) HeroSteps--;
  MoveCount--;
  return 1;
}

// Determine if there's been a win (Theseus at goal w/o loss)
int win(level *l)
{
  if (l->theseus == l->goal) return 1;
  else return 0;
}

// Determine if there's been a loss (Minotaur and Theseus in same square)
int lose(level *l)
{
  if (l->theseus == l->minotaur) return 1;
  else return 0;
}


// write a message
void message(char *msg)
{
  move(0,0);
  clrtoeol();
  mvstr(0,(COLS-strlen(msg))/2,msg);
  move(0,0);
}

// a curses extension: write string at [r,c], clear to end of line
void mvstr(int r, int c, char *s)
{
  move(r,c);
  while (*s) addch(*s++);
  clrtoeol(); // clear the tail of the line
}

// write important statistics to the screen
void updateStats(level *l)
{
  char buffer[80];
  long curTime = time(0);
  int deltaTime = curTime-l->startTime;
  double days,m,e,p;
  int seconds = (int)(deltaTime%60);
  int minutes = (int)((deltaTime/60)%60);
  int hours = (int)((deltaTime/3600)%24);
  int idays = (int)(deltaTime/86400);
  double heroSpeed;
  if (deltaTime)
    heroSpeed = HeroSteps*0.1/deltaTime * 0.056818182; // see man units
  else
    heroSpeed = 0.0;

  sprintf(buffer,"Level: %d    Moves: %d    Time: %+d %d:%02d:%02d   Hero Speed: %8.5lf mph",
	  l->levelNumber, MoveCount, idays, hours, minutes, seconds, heroSpeed);
  mvstr(LINES-3, (COLS-strlen(buffer))/2, buffer);

  // bioindicators
  days = (time(0)-BTime)/86400.0;
  p = sin(days/23.0*2.0*M_PI);
  m = sin(days/33.0*2.0*M_PI);
  e = sin(days/28.0*2.0*M_PI);
  sprintf(buffer,"Physical: %+8.6lf  Emotional: %+8.6lf  Mental: %+8.6lf",p,e,m);
  mvstr(LINES-2,(COLS-strlen(buffer))/2, buffer);
  move(LINES-1,0);
}

// Boss on Deck: print an emacs facade and duck for cover
void OMG(level *lv)
{
  FILE *f = fopen(OMGSCREEN,"r");
  int i,j,ch,l;
  char buffer[100];
  char *status = "-uu-:---F1 gdc.c        All L11     (C-wizard Abbrev)---";
  clear();
  // read the workscreen file, painting as many lines on screen as needed
  for (i = 0; i < LINES-2 && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      ch = buffer[j];
      if (isspace(ch)) ch = ' ';  // trash tabs and newlines
      mvaddch(i,j,ch);
    }
  }
  // now, the status line
  l = strlen(status);
  for (j = 0; j < COLS; j++) {
    if (j >= l) ch = '-';	// past the end of status line; fill with '-'
    else ch = status[j];	// within status line
    mvaddch(LINES-2,j,ch | A_REVERSE); // paint in reverse video
  }
  fclose(f);
  move(12,0); // move cursor to 13th line.  why not?
  refresh(); // paint facade
  // wait for the dust to clear
  getch();
  // repaint the level as it currently is
  clear();
  display(lv);
  updateStats(lv);
  refresh();
}

// print help screen
void help(level *lv)
{
  FILE *f = fopen(HELPSCREEN,"r");
  int i,j,l;
  char buffer[100];
  clear();
  // read the help file painting as many lines on screen as possible.
  for (i = 0; i < LINES && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      mvaddch(i,j,buffer[j]);
    }
  }
  fclose(f);
  move(0,0);
  refresh();
  getch();
  clear();
  display(lv);
  updateStats(lv);
  refresh();
}

// the main method
int main(int argc, char **argv)
{
  level *currentLevel;

  if (argc > 1) currentLevelNumber = atoi(argv[1]);
  else currentLevelNumber = 1;

  // start the curses screen manager
  initialize();
  
  // the play loop.  cranks once per level.
  while (currentLevelNumber <= MAXLEVEL) {
    currentLevel = readLevel(currentLevelNumber);
    display(currentLevel);
    play(currentLevel);
  }
  shutdown();
  return 0;
}
