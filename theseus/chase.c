// A routine for moving the Minotaur
// (c) 2019 <please put your name here>
#include "theseus.h"

// Birthday for advanced biometric tracking:
int BDay[] = { /* this is Duane's: */ 12, 15, 1960 };
int BDay2[] = {11,20,2001};

// Move the minotaur 's' strides toward our hero
int chase(level *l, int s)
{
  int i;
  int moved = 0;
  // move the Minotaur 's' strides; keep track of number of successful moves
  for (i = 0; i < s; i++) {
    moved += moveM(l);
  }
  return moved;
}

// Move the minotaur one stride:
//  * always move toward Theseus
//  * if a horizontal move is possible, take it
//  * otherwise move vertically (if possible)
// Returns 1 if moved, 0 if not.

int moveM(level *l)
{
  int player_x;
  int player_y;
  p2rc(l->minotaur,&player_x,&player_y);
  //Check to see if X position is movable
  int counter = 0;
/*  if (counter == 0){
    (l->minotaur) |
  }*/
  printf("this is x pos: %d ", player_x);
  printf("this is y pos: %d ", player_y);
  movePiece(l,MINOTAUR,0,1,0,2);

  return 0;
}
