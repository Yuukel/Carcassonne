#ifndef DRAW
#define DRAW

#include "game_structures.h"

void RemoveTile(TileStruct* pile);
GameStruct DrawTile(GameStruct game);
void ReorderPile(TileStruct* pile);
int CheckCanBePlaced(GameStruct game, TileStruct t);

#endif  // DRAW