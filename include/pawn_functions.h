#ifndef PAWN
#define PAWN

#include "game_structures.h"

GameStruct ChoosePawnPosition(GameStruct game, int x, int y, int ch);
GameStruct PlacePawn(GameStruct game, CoordStruct coords, int side);
GameStruct AddPawn(GameStruct game);

#endif  // PAWN