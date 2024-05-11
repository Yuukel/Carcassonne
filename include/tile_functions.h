#ifndef TILE
#define TILE

#include <ncurses.h>

#include "game_structures.h"

TurnStruct MoveCamera(TurnStruct turn, int ch);
TileStruct RotateTile(TileStruct t, int ch);
GameStruct PlaceTile(GameStruct game, int ch);
void ReturnCoordsList(GameStruct game, CoordStruct* coords);
GameStruct ChangeNumbers(GameStruct game);

#endif  // TILE