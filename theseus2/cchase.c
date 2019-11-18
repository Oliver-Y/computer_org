// A routine for moving the Minotaur
// (c) 2019 Duane A. Bailey
#include "theseus.h"
#define NIBBLE 0xF
// Birthday for advanced biometric tracking:
int BDay[] = { /* this is Duane's: */ 12, 15, 1960 };

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
  int tr,tc, mr,mc;
  char mch;
  int result = 1;

  tr = getT(l);
  tc = tr & NIBBLE;
  tr = tr >> 4;
  mr = getM(l);
  mc = mr & NIBBLE;
  mr = mr >> 4;
  mch = get(l,mr,mc);
  if (tc < mc && !(mch & LEFT)) {
    movePiece(l,MINOTAUR,mr,mc,mr,mc-1);
  } else if (tc > mc && !(mch & RIGHT)) {
    movePiece(l,MINOTAUR,mr,mc,mr,mc+1);    
  } else if (tr < mr && !(mch & UP)) {
    movePiece(l,MINOTAUR,mr,mc,mr-1,mc);
  } else if (tr > mr && !(mch & DOWN)) {
    movePiece(l,MINOTAUR,mr,mc,mr+1,mc);    
  } else {
    result = 0;
  }
  return result;
}
