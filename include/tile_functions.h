#ifndef TILE
#define TILE

#include <ncurses.h>
#include "game_structures.h"

int WaitingForAction();
int MoveCamera();
TileStruct RotateTile(TileStruct t, int sens);
// pose tuile

#endif // TILE