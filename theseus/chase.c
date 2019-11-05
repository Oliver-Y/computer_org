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
  int minotaur_x;
  int minotaur_y;
  int player_x;
  int player_y;
  int dr = 0;
  int dc = 0;
  int moved = 0;
  int sch;
  p2rc(l->minotaur,&minotaur_x,&minotaur_y);
  p2rc(l->theseus,&player_x,&player_y);
  sch = get(l,minotaur_x,minotaur_y);
  //Figure out the direction
  // if (mintaur_y < player_y) dc = 1; else dc = -1
  // if minotaur_y == player_y) dc = 0;
  // if (minotaurx < playerx) dr = 1, dr = -1
  // if minotaur_x == player_x dr = 0
  dc = minotaur_y < player_y ? 1:-1;
  if(minotaur_y == player_y) dc = 0;
  dr = minotaur_x < player_x ? 1:-1;
  if(minotaur_x == player_x) dr = 0;
  if(!(sch & LEFT) && dc == -1){
    movePiece(l,MINOTAUR,minotaur_x,minotaur_y,minotaur_x,minotaur_y+dc);
    moved = 1;
  }
  else if(!(sch & RIGHT) && dc == 1){
    movePiece(l,MINOTAUR,minotaur_x,minotaur_y,minotaur_x,minotaur_y+dc);
    moved = 1;
  }
  else if(!(sch & DOWN) && dr == 1){
    movePiece(l,MINOTAUR,minotaur_x,minotaur_y,minotaur_x+dr,minotaur_y);
    moved = 1;
  }
  else if(!(sch & UP) && dr == -1){
    movePiece(l,MINOTAUR,minotaur_x,minotaur_y,minotaur_x+dr,minotaur_y);
    moved = 1;
  }
  else{
    moved = 0;
  }
  return moved;
}
